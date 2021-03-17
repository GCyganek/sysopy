CC = gcc -Wall -std=c11 -g

all: zad1 zad1sys

zad1: zad1.c
	$(CC) zad1.c -o zad1

zad1sys: zad1sys.c
	$(CC) zad1sys.c -o zad1sys

test:
	rm -f pomiar_zad_1.txt
	make zad1
	echo "\n./zad1 txt1.txt txt2.txt" >> pomiar_zad_1.txt
	./zad1 txt1.txt txt2.txt
	echo "\n\n\n\n"
	make zad1sys
	echo "\n./zad1sys txt1.txt txt2.txt" >> pomiar_zad_1.txt
	./zad1sys txt1.txt txt2.txt

clean:
	rm -f zad1 zad1sys