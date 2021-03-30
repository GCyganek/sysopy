CC = gcc -Wall -std=c11 -g -D_XOPEN_SOURCE=700

all: main

main: main.c
	$(CC) main.c -o main

test: all
	./main

clean:
	rm -f main