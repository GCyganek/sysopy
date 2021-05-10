CC = gcc -Wall -std=c11 -g -D_XOPEN_SOURCE=700

all: main

main: main.c
	$(CC) main.c -o main -pthread -lm

test: main
	./main 1 numbers totem.ascii.pgm out/totem_numbers_1.ascii.pgm
	./main 2 numbers totem.ascii.pgm out/totem_numbers_2.ascii.pgm
	./main 4 numbers totem.ascii.pgm out/totem_numbers_4.ascii.pgm
	./main 8 numbers totem.ascii.pgm out/totem_numbers_8.ascii.pgm
	./main 16 numbers totem.ascii.pgm out/totem_numbers_16.ascii.pgm

	./main 1 block totem.ascii.pgm out/totem_block_1.ascii.pgm
	./main 2 block totem.ascii.pgm out/totem_block_2.ascii.pgm
	./main 4 block totem.ascii.pgm out/totem_block_4.ascii.pgm
	./main 8 block totem.ascii.pgm out/totem_block_8.ascii.pgm
	./main 16 block totem.ascii.pgm out/totem_block_16.ascii.pgm

clean:
	rm -f main out/*

