CC = gcc -Wall -std=c11 -g -D_POSIX_C_SOURCE=200112L

all: server client

server: server.c common.c
	$(CC) common.c server.c -o server -pthread

client: client.c common.c
	$(CC) common.c client.c -o client -pthread

clean:
	rm -f server client