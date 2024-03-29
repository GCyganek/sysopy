//
// Created by gcyganek on 24.05.2021.
//

#include "common.h"

char* client_name;
int server_socket;
int client_socket;
struct sockaddr_un sockaddr;
char* connect_mode;
char* server_address;
char* server_port;
char is_local;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

typedef enum {
    EMPTY, O, X
} field_val;

typedef enum {
    WAITING_FOR_PLAYER, WAITING_FOR_OPPONENT, RECEIVED_OPPONENTS_CHOICE, END
} game_status;

typedef struct tictactoe {
    field_val board[9];
    game_status status;
} tictactoe;

tictactoe tictactoe_game;

field_val symbol;

char buffer[MAX_MESSAGE_LENGTH + 1];
char *task;
char *task_args;

int check_draw() {
    int draw = 1;
    for (int i = 0; i < 9; i++) {
        if (tictactoe_game.board[i] == EMPTY) {
            draw = 0;
        }
    }
    return draw;
}

field_val check_win() {
    field_val *arr = tictactoe_game.board;
    // checking columns
    for (int i = 0; i < 3; i++) {
        if (arr[i] == arr[i + 3] && arr[i + 3] == arr[i + 6]) return arr[i];
    }

    // checking rows
    for (int i = 0; i < 3; i++) {
        if (arr[3 * i] == arr[3 * i + 1] && arr[3 * i + 1] == arr[3 * i + 2]) return arr[3 * i];
    }

    // checking diagonals
    if (arr[0] == arr[4] && arr[4] == arr[8]) return arr[0];
    if (arr[2] == arr[4] && arr[4] == arr[6]) return arr[2];

    return EMPTY;
}

void check_game_status() {
    field_val winner = check_win();
    if (winner == EMPTY) {
        char draw = check_draw();

        if (draw) {
            printf("Draw!\n");
            tictactoe_game.status = END;
        }
    }

    else if (winner == symbol) {
        printf("You won!\n");
        tictactoe_game.status = END;   
    }

    else {
        printf("You lost!\n");
        tictactoe_game.status = END;
    }
}

void disconnect() {
    char msg[MAX_MESSAGE_LENGTH + 1];
    sprintf(msg, "disconnect| |%s", client_name);
    if (send(server_socket, msg, MAX_MESSAGE_LENGTH, 0) == -1)
        print_error_and_exit("Error while sending disconnect message using send()");
    close(server_socket);
    printf("Disconnecting\n");
    if (is_local) {
        unlink(client_name);
    }
    exit(0);
}

void draw_board() {
    printf("\n\n");
    printf("CURRENT GAME BOARD. Your symbol: %s\n", symbol == 1 ? "O" : "X");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tictactoe_game.board[i * 3 + j] == EMPTY) {
                printf("| -%d- |", (i * 3 + j) + 1);
            }
            
            else if (tictactoe_game.board[i * 3 + j] == O) {
                printf("|  O  |");
            }

            else if (tictactoe_game.board[i * 3 + j] == X) {
                printf("|  X  |");
            }
        }
        printf("\n");
    }
    printf("===============================\n\n");
}

void handle_sigint(int sig_no) {
    (void) sig_no;
    disconnect();
    exit(0);
}

int check_choosen_field(int choosen_field) {
    if (tictactoe_game.board[choosen_field] == EMPTY) {
        tictactoe_game.board[choosen_field] = symbol;
        return 0;
    } else {
        printf("This field is already filled! Choose another field now:\n");
        return -1;
    }
}

void game_loop() {
    while(1) {
        pthread_mutex_lock(&mutex);

        if (tictactoe_game.status == WAITING_FOR_PLAYER) {
            draw_board();
            printf("Waiting for your move... Choose field from 1 to 9 you want to fill\n");

            int choosen_field;
            do {
                pthread_mutex_unlock(&mutex);
                scanf("%d", &choosen_field);
                pthread_mutex_lock(&mutex);
                choosen_field--;
            } while (choosen_field < 0 || choosen_field > 8 || check_choosen_field(choosen_field) == -1);

            check_game_status();

            char msg[MAX_MESSAGE_LENGTH + 1];
            sprintf(msg, "choosen_field|%d|%s", choosen_field, client_name);
            if (send(server_socket, msg, MAX_MESSAGE_LENGTH, 0) == 1) 
                print_error_and_exit("Error while sending choosen_field message to server");

            if (tictactoe_game.status != END) {
                tictactoe_game.status = WAITING_FOR_OPPONENT;
            }
        }

        else if (tictactoe_game.status == WAITING_FOR_OPPONENT) {
            draw_board();
            printf("Waiting for opponent's move...\n");
            
            while (tictactoe_game.status != END && tictactoe_game.status != RECEIVED_OPPONENTS_CHOICE) {
                pthread_cond_wait(&condition, &mutex);
            }
        }

        else if (tictactoe_game.status == RECEIVED_OPPONENTS_CHOICE) {
            int choosen_field = atoi(task_args);
            tictactoe_game.board[choosen_field] = symbol == O ? X : O;

            check_game_status();

            if (tictactoe_game.status != END) {
                tictactoe_game.status = WAITING_FOR_PLAYER;
            }
        }

        else if (tictactoe_game.status == END) {
            draw_board();
            disconnect();
        }
        
        pthread_mutex_unlock(&mutex);
    }
}

void register_to_server_clients_list() {
    char msg[MAX_MESSAGE_LENGTH + 1];
    sprintf(msg, "save| |%s", client_name);
    if (send(server_socket, msg, MAX_MESSAGE_LENGTH, 0) == -1)
        print_error_and_exit("Error while trying to register to server's clients list");
}

void client_loop() {
    while(1) {  
        if (recv(server_socket, buffer, MAX_MESSAGE_LENGTH, 0) == -1)
            print_error_and_exit("Error while listening to server socket using recv()");
        
        // printf("Wiadomosc od servera %s\n", buffer);
        task = strtok(buffer, "|");
        task_args = strtok(NULL, "|");

        pthread_mutex_lock(&mutex);

        if (!strcmp(task, "ping")) {
            char msg[MAX_MESSAGE_LENGTH + 1];
            sprintf(msg, "ping_response| |%s", client_name);
            if (send(server_socket, msg, MAX_MESSAGE_LENGTH, 0) == -1)
                print_error_and_exit("Error while responding to ping using send()");
        }

        else if (!strcmp(task, "save_response")) {
            if (!strcmp(task_args, "no_opponent")) {
                printf("Waiting for opponent...\n");
            }

            else if (!strcmp(task_args, "server_full")) {
                printf("Server is full!\n");
                if (is_local) unlink(client_name);
                exit(1);
            }

            else if (!strcmp(task_args, "name_already_taken")) {
                printf("Your name is already taken!\n");
                exit(1);
            }

            else {
                symbol = task_args[0];

                if (!strcmp(task_args, "O")) {
                    symbol = O;
                } else {
                    symbol = X;
                }

                printf("Your symbol: %s\n", symbol == O ? "O" : "X");

                for (int i = 0; i < 9; i++) {
                    tictactoe_game.board[i] = EMPTY;
                }
                tictactoe_game.status = symbol == O ? WAITING_FOR_PLAYER : WAITING_FOR_OPPONENT;

                pthread_t game_thread;
                if (pthread_create(&game_thread, NULL, (void *(*)(void *))game_loop, NULL) != 0)
                    print_error_and_exit("Error while creating game thread");
            }
        }

        else if (!strcmp(task, "choose_field")) {
            tictactoe_game.status = RECEIVED_OPPONENTS_CHOICE;
            pthread_cond_broadcast(&condition);
        }

        else if (!strcmp(task, "disconnect")) {
            printf("Your opponent has disconnected from the server\n");
            close(server_socket);
            if (is_local) unlink(client_name);
            exit(0);
        }

        else if (!strcmp(task, "server_close")) {
            printf("Server has been closed\n");
            close(server_socket);
            if (is_local) unlink(client_name);
            exit(0);
        }

        else if (!strcmp(task, "no_ping_response")) {
            printf("No ping response\n");
            close(server_socket);
            if (is_local) unlink(client_name);
            exit(0);
        }

        pthread_mutex_unlock(&mutex);
    }
}

void connect_to_network_server() {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    struct addrinfo *res;

    if (getaddrinfo(server_address, server_port, &hints, &res) != 0)
        print_error_and_exit("Error while using getaddrinfo in connect_to_network_server");

    if ((server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) 
        print_error_and_exit("Error while using socket (network) in connect_to_network_server");

    if (connect(server_socket, res->ai_addr, res->ai_addrlen) == -1)
        print_error_and_exit("Error while using connect in connect_to_network_server");

    freeaddrinfo(res);
}

void connect_to_local_server() {
    server_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (server_socket == -1)
        print_error_and_exit("Error while using socket (local) in connect_to_local_server");

    struct sockaddr_un sockaddr;
    memset(&sockaddr, 0, sizeof(struct sockaddr_un));
    sockaddr.sun_family = AF_UNIX;
    strcpy(sockaddr.sun_path, server_address);

    struct sockaddr_un client_sockaddr;
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    client_sockaddr.sun_family = AF_UNIX;
    strcpy(client_sockaddr.sun_path, client_name);

    if (bind(server_socket, (struct sockaddr*)&client_sockaddr, sizeof(client_sockaddr)) == -1)
        print_error_and_exit("Error while using bind (local) in connect_to_local_server");

    if (connect(server_socket, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1)
        print_error_and_exit("Error while using connect in connect_to_local_server");
}

int main(int argc, char** argv) {
    if ((argc !=5 && !strcmp(argv[2], "network")) || (argc != 4 && !strcmp(argv[2], "local"))) {
        fprintf(stderr, "Not a suitable number of program parameters. "
                        "Expected: ./client client_name connect_mode server_address (and server_port if connect_mode == network)\n");
        return 1;
    }

    client_name = argv[1];
    connect_mode = argv[2];
    server_address = argv[3];
    if (!strcmp(connect_mode, "network")) {
        server_port = argv[4];
    }

    signal(SIGINT, handle_sigint);

    if (!strcmp(connect_mode, "network")) {
        connect_to_network_server();
        is_local = 0;
    } else {
        if (!strcmp(connect_mode, "local")) {
            connect_to_local_server();
            is_local = 1;
        } else {
            fprintf(stderr, "Wrong connect_mode given\n");
            exit(1);
        }
    }

    register_to_server_clients_list();

    client_loop();

    return 0;
}