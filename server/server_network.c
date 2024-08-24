#include <stdio.h> 
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <stdlib.h>
#include "colors.h"
#include <unistd.h>
#include <string.h> 
#include "server_network.h"

#define PORT 5900 
#define BUFFER_SIZE (INT32_MAX)


int create_server() {
    int network_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (network_socket < 0) {
        display_red();
        perror("Socket creation failure");
        display_reset();
        exit(EXIT_FAILURE);
    }
    return network_socket;
}


struct sockaddr_in bind_server(int network_socket) { 
    struct sockaddr_in server_socket; 

    server_socket.sin_family = AF_INET;
    server_socket.sin_port = htons(PORT);
    server_socket.sin_addr.s_addr = INADDR_ANY;

    if (bind(network_socket, (struct sockaddr*)&server_socket, sizeof(server_socket)) < 0) {
        display_red();
        perror("Binding Failure");
        display_reset();
        exit(EXIT_FAILURE);
    }

    return server_socket;
}

int listening_connection(int network_socket) {
    if (listen(network_socket, 2) < 0) {
        display_red();
        perror("Listening Failure");
        display_reset();
        exit(EXIT_FAILURE);
    }

    display_magenta();
    printf("Waiting for a connection . . .\n");
    display_reset();
    return 1;
}

int accepting_connection(int network_socket) {
    struct sockaddr_in client_socket;
    memset(&client_socket, 0, sizeof(client_socket));
    int connection = accept(network_socket, (struct sockaddr*)&client_socket, &(socklen_t){sizeof(client_socket)});
    
    if (connection < 0) {
        display_red();
        perror("Acception Failure");
        display_reset();
        exit(EXIT_FAILURE);
    }
    
    display_cyan();
    printf("A client has joined the server!\n");
    display_reset();
    return connection;
}
