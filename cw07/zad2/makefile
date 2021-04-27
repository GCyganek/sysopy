CC = gcc -Wall -std=c11 -g -D_XOPEN_SOURCE=700

all: main cook supplier

main: main.c
	$(CC) main.c -o main -lpthread -lrt

cook: cook.c common.c
	$(CC) common.c cook.c -o cook -lpthread -lrt

supplier: supplier.c common.c
	$(CC) common.c supplier.c -o supplier -lpthread -lrt

clean:
	rm -f main supplier cook