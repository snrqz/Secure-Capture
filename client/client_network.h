#ifndef CLIENT_NETWORK_H
#define CLIENT_NETWORK_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

int start_client(); 
int server_connection(int network_socket);

#endif
