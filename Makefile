CC = gcc
FRAMEWORKS = -framework ApplicationServices
CFLAGS = -Wall -Wextra -Werror

all: server client

server: server.o server_network.o colors.o screen.o
	$(CC) $(CFLAGS) -o server server.o server_network.o colors.o screen.o $(FRAMEWORKS)

client: client.o client_network.o colors.o screen.o
	$(CC) $(CFLAGS) -o client client.o client_network.o colors.o screen.o $(FRAMEWORKS)

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

server_network.o: server_network.c
	$(CC) $(CFLAGS) -c server_network.c

colors.o: colors.c
	$(CC) $(CFLAGS) -c colors.c

screen.o: screen.c
	$(CC) $(CFLAGS) -c screen.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

client_network.o: client_network.c
	$(CC) $(CFLAGS) -c client_network.c

clean:
	rm -f server client *.o
