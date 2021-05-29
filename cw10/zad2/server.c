//
// Created by gcyganek on 24.05.2021.
//

#include "common.h"
#include <poll.h>
#include <time.h>
#include <unistd.h>

#define ACTIVE 1
#define DISABLED 0

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct client {
    char *name;
    int fd;
    char status;
    int opponent_fd;
    int opponent_id;
    struct sockaddr addr;
} client;

int clients_connected = 0;
client *clients[MAX_CLIENTS];

int local_socket;
int network_socket;

int get_client_by_name(char *client_name) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL && !strcmp(clients[i]->name, client_name)) {
            return i;
        }
    }
    return -1;
}

void client_name_already_taken(int client_fd) {
    if (send(client_fd, "save_response|name_already_taken", MAX_MESSAGE_LENGTH, 0) == -1)
        print_error_and_exit("Error while responding to client save message - name already taken");
    close(client_fd);
}

void set_opponent(int client_fd, int client_id) {
    clients[client_id]->opponent_id = -1;
    clients[client_id]->opponent_fd = -1;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (i != client_id && clients[i] != NULL && clients[i]->opponent_fd == -1) {
            clients[i]->opponent_fd = client_fd;
            clients[i]->opponent_id = client_id;
            clients[client_id]->opponent_fd = clients[i]->fd;
            clients[client_id]->opponent_id = i;
            break;
        }
    }
}

void save_client(int client_fd, char *client_name, struct sockaddr client_addr) {
    if (get_client_by_name(client_name) != -1) {
        client_name_already_taken(client_fd);
        return;
    }

    int assigned_index = -1;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == NULL) {
            assigned_index = i;
            break;
        }
    }

    if (assigned_index != -1) {
        client* client = calloc(1, sizeof(struct client));
        client->name = calloc(MAX_MESSAGE_LENGTH, sizeof(char));
        strcpy(client->name, client_name);
        client->fd = client_fd;
        client->addr = client_addr;
        client->status = ACTIVE;
        // printf("%s %d\n", client->name, client->fd);

        clients[assigned_index] = client;
        clients_connected++;

        set_opponent(client_fd, assigned_index);

        // printf("opponent %d \n", client->opponent_fd);

        if (client->opponent_fd == -1) {
            if (sendto(client_fd, "save_response|no_opponent", MAX_MESSAGE_LENGTH, 0,
                       &client_addr, sizeof(struct addrinfo)) == -1)
                print_error_and_exit("Error while responding to client save message - no opponent");
        } else {
            srand(time(NULL));
            int random = rand() % 2;
            int first_player = random ? assigned_index : client->opponent_id;
            int second_player = random ? client->opponent_id : assigned_index;

            if (sendto(clients[first_player]->fd, "save_response|O", MAX_MESSAGE_LENGTH, 0,
                     &clients[first_player]->addr, sizeof(struct addrinfo)) == -1)
                print_error_and_exit("Error while responding to client save message - O assigned");

            if (sendto(clients[second_player]->fd, "save_response|X", MAX_MESSAGE_LENGTH, 0,
                       &clients[second_player]->addr, sizeof(struct addrinfo)) == -1)
                print_error_and_exit("Error while responding to client save message - X assigned");
        }
    }

    else {
        if (sendto(client_fd, "save_response|server_full", MAX_MESSAGE_LENGTH, 0,
                   &client_addr, sizeof(struct addrinfo)) == -1)
            print_error_and_exit("Error while responding to client save message - server full");
    }
}

void remove_client(char *client_name, char *msg) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL && !strcmp(clients[i]->name, client_name)) {
            free(clients[i]->name);
            if (clients[i]->opponent_fd != -1) {
                int opponent_id = clients[i]->opponent_id;
                if (sendto(clients[i]->opponent_fd, msg, MAX_MESSAGE_LENGTH, 0,
                           &clients[opponent_id]->addr, sizeof(struct addrinfo)) == -1)
                    print_error_and_exit("Error while sending disconnect msg to opponent");
                client *opponent = clients[clients[i]->opponent_id];
                free(opponent->name);
                free(opponent);
                clients[clients[i]->opponent_id] = NULL;
                clients_connected--;
            }
            free(clients[i]);
            clients[i] = NULL;
            clients_connected--;
        }
    }
}

void sigint_handle(int signo) {
    (void) signo;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL) {
            if (sendto(clients[i]->fd, "server_close|", MAX_MESSAGE_LENGTH, 0,
                       &clients[i]->addr, sizeof(struct addrinfo)) == -1)
                print_error_and_exit("Error while sending disconnect msg to opponent");
            char msg[MAX_MESSAGE_LENGTH + 1] = "server_close|";
            remove_client(clients[i]->name, msg);
        }
    }
    close(network_socket);
    close(local_socket);
    printf("Server closed successfully...\n");
    exit(0);
}

void remove_disconnected_clients() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL && clients[i]->status == DISABLED) {
            remove_client(clients[i]->name, "no_ping_response|");
        }
    }
}

void test_client_connections() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL) {
            printf("Ping sent to %s\n", clients[i]->name);
            if (sendto(clients[i]->fd, "ping|", MAX_MESSAGE_LENGTH, 0,
                       &clients[i]->addr, sizeof(struct addrinfo)) == -1)
                print_error_and_exit("Error while testing client connection");
            clients[i]->status = DISABLED;
        }
    }
}

void pinging() {
    // printf("Ping loop starting\n");
    while(1) {
        sleep(5);
        printf("Pinging\n");
        pthread_mutex_lock(&clients_mutex);
        remove_disconnected_clients();
        test_client_connections();
        pthread_mutex_unlock(&clients_mutex);
    }
}

int poll_sockets(int network_socket, int local_socket) {
    struct pollfd *pollfds = calloc(2, sizeof(struct pollfd));
    if (pollfds == NULL) 
        print_error_and_exit("Error while allocating memory for pollfds in poll_sockets");

    pollfds[0].fd = network_socket;
    pollfds[1].fd = local_socket;
    pollfds[0].events = pollfds[1].events = POLLIN;

    if (poll(pollfds, 2, -1) == -1)
        print_error_and_exit("Error while using poll in poll_sockets");

    int fd_to_read;
    for (int i = 0; i < 2; i++) {
        if (pollfds[i].revents & POLLIN) {
            fd_to_read = pollfds[i].fd;
            break;
        }
    }

    free(pollfds);
    return fd_to_read;
}

void start_local_socket(char *socket_path) {
    if ((local_socket = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
        print_error_and_exit("Error while using socket in start_local_socket");
    
    struct sockaddr_un name;
    memset(&name, 0, sizeof(name));
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, socket_path, sizeof(name.sun_path) - 1);

    unlink(socket_path);
    if (bind(local_socket, (struct sockaddr *)&name, sizeof(name)) == -1)
        print_error_and_exit("Error while using bind in start_local_socket");
}

void start_network_socket(char *port) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    struct addrinfo *res;

    if (getaddrinfo(NULL, port, &hints, &res) != 0)
        print_error_and_exit("Error while using getaddrinfo in start_network_socket");

    if ((network_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) 
        print_error_and_exit("Error while using socket in start_network_socket");

    if (bind(network_socket, res->ai_addr, res->ai_addrlen) == -1)
        print_error_and_exit("Error while using bind in start_network_socket");

    freeaddrinfo(res);
} 

void server_loop() {
    printf("Server running...\n");
    while(1) {
        int fd_to_read = poll_sockets(network_socket, local_socket);

        printf("descriptor %d\n", fd_to_read);
        struct sockaddr client_addr;
        socklen_t length = sizeof(client_addr);
        char buffer[MAX_MESSAGE_LENGTH + 1];
        if (recvfrom(fd_to_read, buffer, MAX_MESSAGE_LENGTH, 0, &client_addr, &length) == -1)
            print_error_and_exit("Error while reading message from client using recv()");
        
        printf("Received message %s\n", buffer);
        
        char *task = strtok(buffer, "|");
        char *task_args = strtok(NULL, "|");
        char *client_name = strtok(NULL, "|");

        pthread_mutex_lock(&clients_mutex);
        if (!strcmp(task, "save")) {
            save_client(fd_to_read, client_name, client_addr);
        }

        else if (!strcmp(task, "ping_response")) {
            int client_id = get_client_by_name(client_name);
            if (client_id != -1) {
                clients[client_id]->status = ACTIVE;
            }
        }

        else if (!strcmp(task, "disconnect")) {
            remove_client(client_name, "disconnect|");
        }

        else if (!strcmp(task, "choosen_field")) {
            int choosen_field = atoi(task_args);
            int client_id = get_client_by_name(client_name);
            int opponent_id = clients[client_id]->opponent_id;

            char msg[MAX_MESSAGE_LENGTH + 1];
            sprintf(msg, "choose_field|%d", choosen_field);
            if (sendto(clients[opponent_id]->fd, msg, MAX_MESSAGE_LENGTH, 0, &clients[opponent_id]->addr,
                       sizeof(struct addrinfo)) == -1)
                print_error_and_exit("Error while sending choosen_field value to client using send()");
        }
        pthread_mutex_unlock(&clients_mutex);
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Not a suitable number of program parameters. "
                        "Expected: ./server port socket_path \n");
        return 1;
    }

    char* port = argv[1];
    char* socket_path = argv[2];

    signal(SIGINT, sigint_handle);

    memset(&clients, 0, sizeof(clients));

    start_local_socket(socket_path);
    start_network_socket(port);

    pthread_t pinging_thread;
    if (pthread_create(&pinging_thread, NULL, (void* (*)(void*))pinging, NULL) != 0)
        print_error_and_exit("Error while creating pinging thread");

    server_loop();

    return 0;
}