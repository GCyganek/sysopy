CC = gcc -Wall -std=c11 -g

all: main

main: main.c
	$(CC) main.c -o main

test: all
	./main commands.txt

clean:
	rm -f main