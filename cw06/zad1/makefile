CC = gcc -Wall -std=c11 -g -D_XOPEN_SOURCE=700

all: server client

server: server.c header.c
	$(CC) header.c server.c -o server

client: client.c header.c
	$(CC) header.c client.c -o client

clean:
	rm -f server client