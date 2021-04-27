CC = gcc -Wall -std=c11 -g -D_XOPEN_SOURCE=700

all: main cook supplier

main: main.c
	$(CC) main.c -o main

cook: cook.c common.c
	$(CC) common.c cook.c -o cook

supplier: supplier.c common.c
	$(CC) common.c supplier.c -o supplier

clean:
	rm -f main supplier cook