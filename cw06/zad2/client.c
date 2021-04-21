//
// Created by gcyganek on 20.04.2021.
//

#include "header.h"
#include <time.h>

char *queue_filename;
int registration_id;
mqd_t queue_descriptor;
mqd_t connected_queue_descriptor = -1;
mqd_t server_queue_descriptor;

char* generate_random_string(int len) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK1234567890";
    char* random_string = malloc(sizeof(char) * (len + 1));
    sprintf(random_string, "/q-");

    for (int i = 3; i < len; i++) {
        int key = rand() % (int) (sizeof(charset) - 1);
        random_string[i] = charset[key];
    }
    random_string[len] = '\0';
    return random_string;
}

void notification_handler() {
    struct sigevent s_sigevent;
    s_sigevent.sigev_signo = SIGRTMIN;
    s_sigevent.sigev_notify = SIGEV_SIGNAL;

    register_notification(queue_descriptor, &s_sigevent);
}

void register_on_server() {
    queue_filename = generate_random_string(QUEUE_FILENAME_MAX_LEN);
    queue_descriptor = create_queue(queue_filename);
    server_queue_descriptor = get_queue(SERVER_QUEUE_FILENAME);

    send_message_to_queue(server_queue_descriptor, queue_filename, INIT);

    unsigned int type;
    char msg[16];
    get_message_from_queue(queue_descriptor, msg, &type);

    sscanf(msg, "%d", &registration_id);
    printf("registered to the server, received id: %d\n", registration_id);
}

void disconnect() {
    if (connected_queue_descriptor == -1) {
        printf("you are not connected to any chat\n");
        return;
    }

    char msg[2];
    sprintf(msg, "%d", registration_id);
    send_message_to_queue(server_queue_descriptor, msg, DISCONNECT);
    connected_queue_descriptor = -1;

    printf("disconnected from the chat\n");
}

void disconnect_handler() {
    printf("disconnected from the chat\n");
    close_queue(connected_queue_descriptor);
    connected_queue_descriptor = -1;
}

void connect(int client_to_connect_id) {
    if (connected_queue_descriptor != -1) {
        printf("you are already connected to the chat, you have to disconnect first\n");
        return;
    }

    if (client_to_connect_id == registration_id) {
        printf("you can't connect with yourself! try LIST to see available clients you can connect with\n");
        return;
    }

    char msg[5];
    sprintf(msg, "%d %d", registration_id, client_to_connect_id);

    send_message_to_queue(server_queue_descriptor, msg, CONNECT);
    printf("Trying to connect with client %d...\n", client_to_connect_id);
}

void connect_handler(char *msg) {
    if (!strcmp(msg, "-1")) {
        printf("can't connect with client who is already connected, try LIST to see available clients\n");
        return;
    }

    if (!strcmp(msg, "-2")) {
        printf("can't connect with client who does not exist, try LIST to see available clients\n");
        return;
    }

    int connected_client_id;
    char connected_client_queue_filename[16];
    sscanf(msg, "%d %s", &connected_client_id, connected_client_queue_filename);

    connected_queue_descriptor = get_queue(connected_client_queue_filename);
    printf("connection has been established with client %d, you can chat now\n", connected_client_id);
}

void message_handler(char *msg) {
    char text[MAX_MESSAGE_TEXT_LEN];
    int connected_client_id;
    sscanf(msg, "%d %s", &connected_client_id, text);
    printf("MSG FROM CLIENT %d: %s\n", connected_client_id, text);
}

void send_message(char* text) {
    if (connected_queue_descriptor == -1) {
        printf("There is no connection established with another client, you can't chat now\n");
        return;
    }

    char *buf = malloc(sizeof(char) * (strlen(text + 5)));
    sprintf(buf, "%d %s", registration_id, text);
    send_message_to_queue(connected_queue_descriptor, buf, MESSAGE);
}

void list() {
    char msg[2];
    sprintf(msg, "%d", registration_id);
    send_message_to_queue(server_queue_descriptor, msg, LIST);
}

void list_handler(char *list) {
    printf("%s", list);
}

void message_from_queue_handler(int sig_no) {
    (void) sig_no;
    char *msg = malloc(sizeof(char) * MAX_MESSAGE_TEXT_LEN);
    unsigned int type;
    get_message_from_queue(queue_descriptor, msg, &type);

    switch (type) {
        case STOP:
            exit(0);
        case DISCONNECT:
            disconnect_handler();
            break;
        case CONNECT:
            connect_handler(msg);
            break;
        case LIST:
            list_handler(msg);
            break;
        case MESSAGE:
            message_handler(msg);
            break;
        default:
            break;
    }

    notification_handler();
    free(msg);
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

void stop_client() {
    if (connected_queue_descriptor != -1) {
        disconnect();
    }

    char msg[2];
    sprintf(msg, "%d", registration_id);
    send_message_to_queue(server_queue_descriptor, msg, STOP);

    close_queue(queue_descriptor);
    close_queue(server_queue_descriptor);

    delete_queue(queue_filename);
    printf("client stopped\n");
}

void clean() {
    stop_client();
    free(queue_filename);
}

void sigint_handler(int sig_no) {
    (void) sig_no;
    exit(0);
}

int main() {
    atexit(clean);

    srand(time(NULL));

    struct sigaction sigact;
    sigact.sa_handler = message_from_queue_handler;
    sigact.sa_flags = 0;
    sigemptyset(&sigact.sa_mask);
    sigaction(SIGRTMIN, &sigact, NULL);

    signal(SIGINT, sigint_handler);

    register_on_server();

    notification_handler();

    printf("use commands: STOP | CONNECT [client_id] | DISCONNECT | LIST | MESSAGE [msg_text]\n\n");

    char line[MAX_MESSAGE_TEXT_LEN];
    while(1) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            continue;
        }
        input_from_client_handler(line);
    }
}