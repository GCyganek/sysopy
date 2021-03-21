CC = gcc -Wall -std=c11 -g

all: zad5 zad5sys

zad5: zad5.c
	$(CC) zad5.c -o zad5

zadsys5: zad5sys.c
	$(CC) zad5sys.c -o zad5sys

test:
	rm -f pomiar_zad_5.txt
	make zad5
	echo "\n./zad5 txt.txt result.txt" >> pomiar_zad_5.txt
	./zad5 txt.txt result.txt
	make zad5sys
	echo "\n./zad5sys txt.txt resultsys.txt" >> pomiar_zad_5.txt
	./zad5sys txt.txt resultsys.txt

clean:
	rm -f zad5 zad5sys result.txt resultsys.txt