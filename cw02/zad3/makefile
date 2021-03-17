CC = gcc -Wall -std=c11 -g

all: zad3 zad3sys

zad3sys: zad3sys.c
	$(CC) zad3sys.c -o zad3sys -lm

zad3: zad3.c
	$(CC) zad3.c -o zad3 -lm

test:
	rm -f pomiar_zad_3.txt
	make clean
	make zad3sys
	echo "\n./zad3sys" >> pomiar_zad_3.txt
	./zad3sys
	make zad3
	echo "\n.zad3" >> pomiar_zad_3.txt
	./zad3

clean:
	rm -f zad3 a.txt b.txt c.txt zad3sys asys.txt bsys.txt csys.txt