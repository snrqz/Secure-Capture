#ifndef CLIENT_H
#define CLIENT_H

void secure_ss(size_t key, int socket_network);
void ss(int socket_network);
void* input_insertion(void* arg);
void* existing_connection(void* arg);

#endif 
