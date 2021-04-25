//
// Created by gcyganek on 19.04.2021.
//

#include "header.h"

int client_queue_id;
int server_queue_id;
int registration_id;
int connected_client_queue_id = -1;
pid_t connected_client_pid;

void connect(int client_to_connect_id) {
    if (connected_client_queue_id != -1) {
        printf("you are already connected to the chat, you have to disconnect first\n");
        return;
    }

    if (client_to_connect_id == registration_id) {
        printf("you can't connect with yourself! try LIST to see available clients you can connect with\n");
        return;
    }
    message msg;
    msg.sender_id = registration_id;
    msg.msg_type = CONNECT;
    sprintf(msg.msg_text, "%d", client_to_connect_id);

    send_message_to_queue(server_queue_id, &msg);
    printf("Trying to connect with client %d...\n", client_to_connect_id);
}

void disconnect() {
    if (connected_client_queue_id == -1) {
        printf("you are not connected to any chat\n");
        return;
    }
    message msg;
    msg.msg_type = DISCONNECT;
    msg.sender_pid = getpid();
    msg.sender_id = registration_id;

    send_message_to_queue(server_queue_id, &msg);
    connected_client_queue_id = -1;

    printf("disconnected from the chat\n");
}

void disconnect_handler() {
    printf("disconnected from the chat\n");
    connected_client_queue_id = -1;
}

void register_on_server() {
    key_t public_key = get_public_key();
    server_queue_id = get_queue_id(public_key);

    key_t private_key = get_private_key();
    client_queue_id = create_queue(private_key);

    message msg;
    msg.msg_type = INIT;
    msg.sender_pid = getpid();
    sprintf(msg.msg_text, "%d", client_queue_id);

    send_message_to_queue(server_queue_id, &msg);

    get_message_from_queue(client_queue_id, &msg);

    registration_id = atoi(msg.msg_text);
    printf("registered to the server, received id: %d\n", registration_id);
}

void stop_client() {
    message stop_msg;
    stop_msg.msg_type = STOP;
    stop_msg.sender_pid = getpid();
    stop_msg.sender_id = registration_id;

    if (connected_client_queue_id != -1) {
        disconnect();
    }

    send_message_to_queue(server_queue_id, &stop_msg);


    delete_queue(client_queue_id);
    printf("client stopped\n");
}

void connect_handler(message* msg) {
    if (msg->sender_id == -1) {
        printf("can't connect with client who is already connected, try LIST to see available clients\n");
        return;
    }

    if (msg->sender_id == -2) {
        printf("can't connect with client who does not exist, try LIST to see available clients\n");
        return;
    }

    int connected_client_id = msg->sender_id;
    connected_client_pid = msg->sender_pid;
    connected_client_queue_id = atoi(msg->msg_text);
    printf("connection with client %d has been established, you can chat now\n", connected_client_id);
}

void message_handler(message* msg) {
    printf("MSG FROM CLIENT %d: %s\n", msg->sender_id, msg->msg_text);
}

void send_message(char* text) {
    if (connected_client_queue_id == -1) {
        printf("There is no connection established with another client, you can't chat now\n");
        return;
    }
    message msg;
    msg.sender_id = registration_id;
    msg.msg_type = MESSAGE;
    msg.sender_pid = getpid();
    msg.sender_id = registration_id;
    sprintf(msg.msg_text, "%s", text);

    send_message_to_queue(connected_client_queue_id, &msg);
    kill(connected_client_pid, SIGRTMIN);
}

void list() {
    message msg;
    msg.sender_id = registration_id;
    msg.msg_type = LIST;
    msg.sender_pid = getpid();

    send_message_to_queue(server_queue_id, &msg);
}

void list_handler(message *list) {
    printf("%s", list->msg_text);
}

void input_from_client_handler(char* line) {
    char* first_word;
    char* context;

    first_word = strtok_r(line, " \n", &context);

    if (!strcmp("STOP", first_word)) {
        exit(0);
    }

    else if (!strcmp("DISCONNECT", first_word)) {
        disconnect();
    }

    else if (!strcmp("CONNECT", first_word)) {
        connect(atoi(context));
    }

    else if (!strcmp("LIST", first_word)) {
        list();
    }

    else if (!strcmp("MESSAGE", first_word)) {
        send_message(context);
    }

    else {
        printf("Wrong command name\n");
    }
}

void clean() {
    stop_client();
}

void sigint_handler(int sig_no) {
    (void) sig_no;
    exit(0);
}

void message_from_queue_handler(int sig_no) {
    (void) sig_no;
    message msg;
    get_message_from_queue_without_waiting(client_queue_id, &msg);

    switch (msg.msg_type) {
        case STOP:
            exit(0);
        case DISCONNECT:
            disconnect_handler();
            break;
        case CONNECT:
            connect_handler(&msg);
            break;
        case LIST:
            list_handler(&msg);
            break;
        case MESSAGE:
            message_handler(&msg);
            break;
        default:
            break;
    }
}

int main() {
    atexit(clean);

    struct sigaction sigact;
    sigact.sa_handler = message_from_queue_handler;
    sigact.sa_flags = 0;
    sigemptyset(&sigact.sa_mask);
    sigaction(SIGRTMIN, &sigact, NULL);

    signal(SIGINT, sigint_handler);
    register_on_server();

    printf("use commands: STOP | CONNECT [client_id] | DISCONNECT | LIST | MESSAGE [msg_text]\n\n");

    char line[MAX_MESSAGE_TEXT_LEN];
    while(1) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            continue;
        }
        input_from_client_handler(line);
    }
}