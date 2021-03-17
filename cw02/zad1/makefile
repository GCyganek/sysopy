CC = gcc -Wall -std=c11 -g

all: zad1

zad1: zad1.c
	$(CC) zad1.c -o zad1

test:
	make zad1
	./zad1 txt1.txt txt2.txt

clean:
	rm -f zad1