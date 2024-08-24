#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <pthread.h>
#include "screen.h"
#include "server_network.h"
#include "colors.h"
#include "server.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#define BUFFER_SIZE (INT32_MAX)

#define IMAGE_FILE "received_image.raw"

int connection_available = 1;
int global_network_socket;
int global_client_socket;

pthread_mutex_t shutdown_mutex = PTHREAD_MUTEX_INITIALIZER;
int shutdown_trigger = 0;

int main() {
    global_network_socket = create_server();
    bind_server(global_network_socket);
    listening_connection(global_network_socket);
    global_client_socket = accepting_connection(global_network_socket);
    pthread_t server_processor, image_processor, send_connection;

    if (pthread_create(&server_processor, NULL, receive_input, NULL) != 0) {
        display_red();
        printf("Pthread error");
        display_reset();
    }

    if (pthread_create(&image_processor, NULL, receive_forever, NULL) != 0) {
        display_red();
        printf("Pthread error");
        display_reset();
    }

    if (pthread_create(&send_connection, NULL, send_connection_available, NULL) != 0) {
        display_red();
        printf("Pthread error");
        display_reset();
    }

    pthread_join(send_connection, NULL);
    pthread_join(server_processor, NULL);
    pthread_join(image_processor, NULL);

    close(global_network_socket);
    close(global_client_socket);

    return 0;
}

void receive_and_display_image() {
    if (global_client_socket == -1) {
        return;
    }

    char *buffer = (char*)malloc(BUFFER_SIZE);

    if (!buffer) {
        perror("Allocation Failure");
        return;
    }

    size_t image_size = 0;
    ssize_t size_received = recv(global_client_socket, &image_size, sizeof(image_size), 0);

    pthread_mutex_lock(&shutdown_mutex);
    if (size_received != sizeof(image_size)) {
        free(buffer);
        if (!shutdown_trigger) { 
            shutdown_trigger = 1;
            socket_shutdown();
        }
        pthread_mutex_unlock(&shutdown_mutex);
        return;
        // perror("Failed to receive image size");
    }
    pthread_mutex_unlock(&shutdown_mutex);

    image_size = ntohl(image_size);

    printf("Image size to receive: %zu bytes\n", image_size);

    size_t total_received = 0;
    ssize_t bytes_received;

    while (total_received < image_size) {
        bytes_received = recv(global_client_socket, buffer + total_received, BUFFER_SIZE - total_received, 0);

        if (bytes_received < 0) {
            perror("Failed to receive data");
            free(buffer);
            return;

        } else if (bytes_received == 0) {
            fprintf(stderr, "Client disconnected\n");
            close(global_client_socket);
            global_client_socket = -1;
            break;
        }

        total_received += bytes_received;

        printf("Bytes received: %zu / %zu\n", total_received, image_size);
    }

    if (total_received > 0) {
        FILE *file = fopen(IMAGE_FILE, "wb");
        if (file) {
            fwrite(buffer, 1, total_received, file);
            fclose(file);
            open_image_background(); 
        } else {
            perror("Failed to open file");
        }
    } else {
        fprintf(stderr, "No data received\n");
    }

    free(buffer);
}


void open_image_background() {
    pid_t pid = fork();

    if (pid == 0) {
        execlp("open", "open", IMAGE_FILE, (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        sleep(1);
        waitpid(pid, NULL, 0);
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}


void* receive_input(void* arg __attribute__((unused))) {
    char* input = (char*)malloc(sizeof(char) * 100);
    if (input == NULL) {
        perror("malloc");
        pthread_exit(NULL);
    }
    do {
        fgets(input, 100, stdin);
        pthread_mutex_lock(&shutdown_mutex);
        if (strcmp(input, "QUIT\n") == 0 || strcmp(input, "SHUTDOWN\n") == 0) {
            if (!shutdown_trigger) {
                shutdown_trigger = 1;
                socket_shutdown();
            }
            pthread_mutex_unlock(&shutdown_mutex);
            break;
        }
        pthread_mutex_unlock(&shutdown_mutex);
        memset(input, 0, 100);
    } while (global_client_socket != -1 && global_network_socket != -1);

    free(input);
    pthread_exit(NULL);
}


void* receive_forever(void* arg __attribute__((unused))) {
    do {
        receive_and_display_image();
    } while (global_client_socket != -1 && global_network_socket != -1);

    pthread_mutex_lock(&shutdown_mutex);
    if (!shutdown_trigger) {
        shutdown_trigger = 1;
        socket_shutdown();
    }
    pthread_mutex_unlock(&shutdown_mutex);
    pthread_exit(NULL);
}

void socket_shutdown() {
    display_red();
    printSHUTDOWN();
    display_reset();
    close(global_client_socket);
    close(global_network_socket);
    global_network_socket = -1;
    global_client_socket = -1;
}

void* send_connection_available(void* arg __attribute__((unused))) {
    do {
        send(global_client_socket, &connection_available, sizeof(connection_available), 0);
    } while (global_client_socket != -1 && global_network_socket != -1);
    return NULL;
}

// irrelevant atm nevertheless, could possibly be used for unix os 

// void display_image(const char* filename) {
//     char command[256];
//     snprintf(command, sizeof(command), "open %s", filename);
//     system(command);
//     sleep(5);
//     system("killall Preview");
// }
