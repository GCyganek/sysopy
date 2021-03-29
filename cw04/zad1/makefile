CC = gcc -Wall -std=c11 -g -D_XOPEN_SOURCE=700

all: exec main

main: main.c
	$(CC) main.c -o main

exec: exec.c
	$(CC) exec.c -o exec

test: all
	./main handler fork
	./main ignore fork
	./main mask fork
	./main pending fork
	./main ignore exec
	./main mask exec
	./main pending exec

clean:
	rm -f exec main