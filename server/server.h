#ifndef SERVER_H
#define SERVER_H

void socket_shutdown();
void receive_and_display_image();
void* receive_input(void* arg);
void* receive_forever(void* arg);
void open_image_background();
void* send_connection_available(void* arg);

#endif
