CC = gcc -Wall -std=c11 -g -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200112

all: main

main: main.c
	$(CC) main.c -o main -pthread

main60perc: main60perc.c
	$(CC) main60perc.c -o main60perc -pthread

test: main
	./main

test60perc: main60perc
	./main60perc

clean:
	rm -f main main60perc

