CC = gcc -Wall -std=c11 -g

all: zad4sys zad4

zad4sys: zad4sys.c
	$(CC) zad4sys.c -o zad4sys

zad4: zad4.c
	$(CC) zad4.c -o zad4

test:
	rm -f pomiar_zad_4.txt
	make zad4
	rm -f result.txt result2.txt
	echo "\n./zad4 txt.txt result.txt sysopy SYSOPY" >> pomiar_zad_4.txt
	./zad4 txt.txt result.txt sysopy SYSOPY
	echo "\n./zad4 txt.txt result.txt sysopy ." >> pomiar_zad_4.txt
	./zad4 txt.txt result2.txt sysopy .
	make zad4sys
	rm -f resultsys.txt resultsys2.txt
	echo "\n./zad4sys txt.txt result.txt sysopy SYSOPY" >> pomiar_zad_4.txt
	./zad4sys txt.txt resultsys.txt sysopy SYSOPY
	echo "\n./zad4sys txt.txt result.txt sysopy ." >> pomiar_zad_4.txt
	./zad4sys txt.txt resultsys2.txt sysopy .

clean:
	rm -f zad4 zad4sys result.txt result2.txt resultsys.txt resultsys2.txt