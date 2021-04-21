//
// Created by gcyganek on 20.04.2021.
//

#include "header.h"

#define MAX_CLIENTS 10

mqd_t server_queue_descriptor;
int clients_counter = 0;
client* clients[MAX_CLIENTS] = {NULL};

int get_first_free_client_id() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == NULL) {
            return i;
        }
    }
    return -1;
}

int check_client_active(int client_id) {
    if (clients[client_id] == NULL) {
        return -1;
    }
    return 0;
}

int check_client_available(int client_id) {
    if (!clients[client_id]->is_available) {
        return -1;
    }
    return 0;
}


void connect_handler(char *msg) {
    int client1_id;
    int client2_id;

    sscanf(msg, "%d %d", &client1_id, &client2_id);

    client* client1 = clients[client1_id];

    if (clients[client2_id] == NULL) {
        printf("Can't connect client %d with client %d: No such client %d is active\n",
               client1_id, client2_id, client2_id);
        char msg[] = "-2";
        send_message_to_queue(client1->queue_descriptor, msg, CONNECT);
        return;
    }
    client* client2 = clients[client2_id];

    if (!client2->is_available) {
        printf("Can't connect %d with client %d: client %d is already connected with another\n",
               client1_id, client2_id, client2_id);
        char msg[] = "-1";
        send_message_to_queue(client1->queue_descriptor, msg, CONNECT);
        return;
    }

    client1->connected_client_id = client2_id;
    client2->connected_client_id = client1_id;
    client1->is_available = 0;
    client2->is_available = 0;

    char msg1[32];
    sprintf(msg1, "%d %s", client1_id, client1->queue_filename);
    send_message_to_queue(client2->queue_descriptor, msg1, CONNECT);

    char msg2[32];
    sprintf(msg2, "%d %s", client2_id, client2->queue_filename);
    send_message_to_queue(client1->queue_descriptor, msg2, CONNECT);

    printf("connection between client %d and client %d established\n", client1_id, client2_id);
}

void list_handler(char *msg) {
    int client_id;

    sscanf(msg, "%d", &client_id);

    char* list = calloc(MAX_MESSAGE_TEXT_LEN, sizeof(char));
    int list_len = 0;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (i == client_id) continue;

        if (!check_client_active(i)) {
            sprintf(list + list_len, "%d - ", clients[i]->id);
            list_len = strlen(list);

            if (!check_client_available(i)) {
                sprintf(list + list_len, "available\n");
            }

            else {
                sprintf(list + list_len, "not available\n");
            }

            list_len = strlen(list);
        }
    }

    if (list_len == 0) {
        sprintf(list, "no clients available\n");
    }

    send_message_to_queue(clients[client_id]->queue_descriptor, list, LIST);
    printf("list to client %d has been sent\n", client_id);
    free(list);
}

void disconnect_handler(char *msg) {
    int client1_id;

    sscanf(msg, "%d", &client1_id);

    client* client1 = clients[client1_id];
    client* client2 = clients[client1->connected_client_id];

    client1->is_available = 1;
    client2->is_available = 1;

    send_message_to_queue(client2->queue_descriptor, "", DISCONNECT);
    printf("connection between client %d and client %d ended\n", client1_id, client1->connected_client_id);
    client1->connected_client_id = -1;
    client2->connected_client_id = -1;
}

void stop_handler(char *msg) {
    int client_to_delete_id;

    sscanf(msg, "%d", &client_to_delete_id);

    client* client = clients[client_to_delete_id];
    close_queue(client->queue_descriptor);
    free(client);
    clients[client_to_delete_id] = NULL;
    clients_counter--;
    printf("client %d stopped\n", client_to_delete_id);
}

void init_handler(char *msg) {
    int new_client_id = get_first_free_client_id();
    if (new_client_id == -1) {
        print_error_to_stderr_and_quit("Too many clients already registered.\n");
    }

    client* client = calloc(1, sizeof(client));
    client->id = new_client_id;
    client->connected_client_id = -1;
    client->is_available = 1;
    client->queue_filename = malloc(sizeof(char) * strlen(msg));

    sscanf(msg, "%s", client->queue_filename);
    client->queue_descriptor = get_queue(client->queue_filename);


    clients[new_client_id] = client;
    clients_counter++;

    char reply_msg[3];
    sprintf(reply_msg, "%d", new_client_id);
    send_message_to_queue(client->queue_descriptor, reply_msg, INIT);
    printf("client %d created\n", new_client_id);
}

void init_server_queue() {
    server_queue_descriptor = create_queue(SERVER_QUEUE_FILENAME);
    printf("server initialized\n");
}

void stop_server() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL) {
            send_message_to_queue(clients[i]->queue_descriptor, "", STOP);
        }
    }

    close_queue(server_queue_descriptor);
    delete_queue(SERVER_QUEUE_FILENAME);
    printf("server stopped\n");
}

void sigint_handler(int sig_no) {
    (void) sig_no;
    exit(0);
}

void clean() {
    stop_server();
}

int main() {
    atexit(clean);

    signal(SIGINT, sigint_handler);
    init_server_queue();

    while(1) {
        char *msg = malloc(sizeof(char) * MAX_MESSAGE_TEXT_LEN);
        unsigned int type;
        get_message_from_queue(server_queue_descriptor, msg, &type);

        switch (type) {
            case STOP:
                stop_handler(msg);
                break;
            case DISCONNECT:
                disconnect_handler(msg);
                break;
            case LIST:
                list_handler(msg);
                break;
            case CONNECT:
                connect_handler(msg);
                break;
            case INIT:
                init_handler(msg);
                break;
            default:
                break;
        }
        free(msg);
    }
}