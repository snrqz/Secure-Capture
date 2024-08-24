#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include "colors.h"
#include <string.h> 

#define PORT 5900


int start_client() {
    int network_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (network_socket < 0) {
        display_red();
        perror("Failed to create a socket");
        display_reset();
        exit(EXIT_FAILURE);
    }
    return network_socket;
}


int server_connection(int network_socket) { 
    struct sockaddr_in client_addr; 
    memset(&client_addr, 0, sizeof(client_addr));

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT);
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(network_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        display_red();
        perror("Failed to connect\n");
        display_reset();
        exit(EXIT_FAILURE);
    }

    return 0;
}

