#include <sys/socket.h>

#ifndef SERVER_NETWORK_H
#define SERVER_NETWORK_H

int create_server();
struct sockaddr_in bind_server(int network_socket);
int listening_connection(int network_socket);
int accepting_connection(int network_socket);

#endif
