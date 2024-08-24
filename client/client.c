#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <pthread.h>
#include "colors.h"
#include "screen.h"
#include "client_network.h"
#include "client.h"
#include <unistd.h>

#define BUFFER_SIZE (INT32_MAX)

int socket_network;
int connection_available; 
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    socket_network = start_client();
    server_connection(socket_network);
    pthread_t client_processor, connection_loop;

    if (pthread_create(&client_processor, NULL, input_insertion, (void*)(intptr_t)socket_network) != 0) {
        display_red();
        perror("Pthread creation error");
        display_reset();
    }

    if (pthread_create(&connection_loop, NULL, existing_connection, NULL) != 0) {
        display_red();
        perror("Pthread creation error");
        display_reset();
    }

    pthread_join(connection_loop, NULL);
    pthread_join(client_processor, NULL);

    return 0;
}

void secure_ss(size_t key, int socket_network) {
    if (key == 0) {
        display_red();
        printf("Xor key 0 // invalid (encryption)\n");
        display_reset();
        exit(EXIT_FAILURE);
    }
    char* buffer = (char*)malloc(BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) {
        display_red();
        printf("Memory allocation failed\n");
        display_reset();
        exit(EXIT_FAILURE);
    }

    size_t data_length = capture_screen(buffer, BUFFER_SIZE);

    for (size_t i = 0; i < data_length; i++) {
        buffer[i] = buffer[i] ^ key;
    }
    
    size_t size_network_order = htonl(data_length);
    send(socket_network, &size_network_order, sizeof(size_network_order), 0);
    display_cyan();
    printf("Sent %ld Encrypted Bytes\n", data_length);
    display_reset();

    size_t total_sent = 0;
    ssize_t bytes_sent;

    while (total_sent < data_length) { 
        bytes_sent = send(socket_network, buffer + total_sent, data_length - total_sent, 0);
        if (bytes_sent < 0) {
            display_red();
            perror("Failed to send data");
            display_reset();
            free(buffer);
            exit(EXIT_FAILURE);
        }
        total_sent += bytes_sent;
    }

    free(buffer);
}

void ss(int socket_network) {
    char* buffer = (char*)malloc(BUFFER_SIZE);
    if (buffer == NULL)  {
        display_red();
        printf("Memory allocation failed\n");
        display_reset();
        exit(EXIT_FAILURE);
    }
    memset(buffer, 0, BUFFER_SIZE);

    size_t data_length = capture_screen(buffer, BUFFER_SIZE);

    size_t size_network_order = htonl(data_length); 
    send(socket_network, &size_network_order, sizeof(size_network_order), 0);

    display_cyan();
    printf("Sent %ld Bytes\n", data_length);
    display_reset();

    size_t total_sent = 0;
    ssize_t bytes_sent;

    while (total_sent < data_length) {
        bytes_sent = send(socket_network, buffer + total_sent, data_length - total_sent, 0);
        if (bytes_sent < 0) {
            display_red();
            perror("Failed to send data");
            display_reset();
            free(buffer);
            exit(EXIT_FAILURE);
        }
        total_sent += bytes_sent;
    }
    free(buffer);
}



void* input_insertion(void* arg) {
    int socket_network = (int)(intptr_t)arg;
    char* input = (char*)malloc(100 * sizeof(char)); 
    int choice = 1;
    if (input == NULL) {
        display_red();
        printf("Memory allocation failed\n");
        display_reset();
        exit(EXIT_FAILURE);
    }
    memset(input, 0, 100);
    do {
        pthread_mutex_lock(&client_mutex);
        if (socket_network == -1) {
            pthread_mutex_unlock(&client_mutex);
            break;
        }
        pthread_mutex_unlock(&client_mutex);
        fgets(input, 100, stdin);
        if (strcmp(input, "EXIT\n") == 0 || strcmp(input, "QUIT\n") == 0)
            break;
        else if (strcmp(input, "SS\n") == 0 || strcmp(input, "ss\n") == 0) {
            ss(socket_network);
        }
        else if (strcmp(input, "securess\n") == 0 || strcmp(input, "SECURESS\n") == 0) {
            display_green();
            printf("Insert a key to encrypt with:\n");
            scanf("%d", &choice); 
            display_reset();
            secure_ss(choice, socket_network);  
        }

        memset(input, 0, 100);
    } while (socket_network != -1);

    if (socket_network != -1) {
        close(socket_network);
        socket_network = -1;
    }

    free(input); 
    pthread_exit(NULL);
}

void* existing_connection(void* arg __attribute__((unused))) {
    ssize_t bytes_received;
    while ((bytes_received = recv(socket_network, &connection_available, sizeof(connection_available), 0)) > 0) {
    }

    if (socket_network != -1) {
        close(socket_network);
        socket_network = -1;
    }
    exit(EXIT_SUCCESS);
    return NULL;
}

