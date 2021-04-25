//
// Created by gcyganek on 19.04.2021.
//

#include "header.h"

#define MAX_CLIENTS 10

int server_queue_id;
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

void init_handler(message* msg) {
    int new_client_id = get_first_free_client_id();
    if (new_client_id == -1) {
        fprintf(stderr, "Too many clients already registered.\n");
        exit(EXIT_FAILURE);
    }

    client* client = calloc(1, sizeof(client));
    client->client_id = new_client_id;
    client->client_queue_id = atoi(msg->msg_text);
    client->connected_client_id = -1;
    client->is_available = 1;
    client->client_pid = msg->sender_pid;

    clients[new_client_id] = client;
    clients_counter++;

    message reply_msg;
    reply_msg.msg_type = INIT;
    sprintf(reply_msg.msg_text, "%d", new_client_id);
    send_message_to_queue(client->client_queue_id, &reply_msg);
    printf("client %d created\n", new_client_id);
}

void stop_handler(message* msg) {
    int client_to_delete_id = msg->sender_id;
    client* client = clients[client_to_delete_id];
    clients[client_to_delete_id] = NULL;
    clients_counter--;
    free(client);
    printf("client %d stopped\n", client_to_delete_id);
}

void disconnect_handler(message* msg) {
    int client1_id = msg->sender_id;
    client* client1 = clients[client1_id];
    client* client2 = clients[client1->connected_client_id];

    client1->is_available = 1;
    client2->is_available = 1;

    message reply_msg;
    reply_msg.msg_type = DISCONNECT;
    send_message_to_queue(client2->client_queue_id, &reply_msg);
    kill(client2->client_pid, SIGRTMIN);
    printf("connection between client %d and client %d ended\n", client1_id, client1->connected_client_id);
    client1->connected_client_id = -1;
    client2->connected_client_id = -1;
}

void connect_handler(message* msg) {
    int client1_id = msg->sender_id;
    client* client1 = clients[client1_id];
    int client2_id = atoi(msg->msg_text);

    message reply_msg;
    reply_msg.msg_type = CONNECT;

    if (clients[client2_id] == NULL) {
        printf("Can't connect client %d with client %d: No such client %d is active\n",
               client1_id, client2_id, client2_id);
        reply_msg.sender_id = -2;
        send_message_to_queue(client1->client_queue_id, &reply_msg);
        kill(client1->client_pid, SIGRTMIN);
        return;
    }
    client* client2 = clients[client2_id];

    if (!client2->is_available) {
        printf("Can't connect %d with client %d: client %d is already connected with another\n",
               client1_id, client2_id, client2_id);
        reply_msg.sender_id = -1;
        send_message_to_queue(client1->client_queue_id, &reply_msg);
        kill(client1->client_pid, SIGRTMIN);
        return;
    }

    client1->connected_client_id = client2_id;
    client2->connected_client_id = client1_id;
    client1->is_available = 0;
    client2->is_available = 0;

    reply_msg.sender_id = client1_id;
    reply_msg.sender_pid = client1->client_pid;
    sprintf(reply_msg.msg_text, "%d", client1->client_queue_id);
    send_message_to_queue(client2->client_queue_id, &reply_msg);
    kill(client2->client_pid, SIGRTMIN);
    reply_msg.sender_id = client2_id;
    reply_msg.sender_pid = client2->client_pid;
    sprintf(reply_msg.msg_text, "%d", client2->client_queue_id);
    send_message_to_queue(client1->client_queue_id, &reply_msg);
    kill(client1->client_pid, SIGRTMIN);

    printf("connection between client %d and client %d established\n", client1_id, client2_id);
}

void list_handler(message* msg) {
    int client_id = msg->sender_id;
    client* client = clients[client_id];

    message reply_msg;
    reply_msg.msg_type = LIST;

    char* list = calloc(MAX_MESSAGE_TEXT_LEN, sizeof(char));
    int list_len = 0;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (i == client_id) continue;

        if (!check_client_active(i)) {
            sprintf(list + list_len, "%d - ", clients[i]->client_id);
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

    sprintf(reply_msg.msg_text, "%s", list);
    send_message_to_queue(client->client_queue_id, &reply_msg);
    kill(client->client_pid, SIGRTMIN);
    printf("list to client %d has been sent\n", client_id);
}

void stop_server() {
    message stop_clients;
    stop_clients.msg_type = STOP;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL) {
            send_message_to_queue(clients[i]->client_queue_id, &stop_clients);
            kill(clients[i]->client_pid, SIGRTMIN);
        }
    }

    while (clients_counter > 0) {
        message client_answer;
        get_message_from_queue(server_queue_id, &client_answer);
        stop_handler(&client_answer);
    }

    delete_queue(server_queue_id);
    printf("server stopped\n");
}

void init_server_queue() {
    key_t public_key = get_public_key();
    server_queue_id = create_queue(public_key);

    printf("server initialized\n");
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

    message msg;
    while(1) {
        get_message_from_queue(server_queue_id, &msg);

        switch (msg.msg_type) {
            case STOP:
                stop_handler(&msg);
                break;
            case DISCONNECT:
                disconnect_handler(&msg);
                break;
            case LIST:
                list_handler(&msg);
                break;
            case CONNECT:
                connect_handler(&msg);
                break;
            case INIT:
                init_handler(&msg);
                break;
            default:
                break;
        }
    }
}