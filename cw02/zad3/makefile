CC = gcc -Wall -std=c11 -g

all: zad3

zad3: zad3.c
	$(CC) zad3.c -o zad3 -lm

test:
	make clean
	make zad3
	./zad3

clean:
	rm -f zad3 a.txt b.txt c.txt