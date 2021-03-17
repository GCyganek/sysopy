CC = gcc -Wall -std=c11 -g

all: zad4

zad4: zad4.c
	$(CC) zad4.c -o zad4

test:
	make zad4
	rm -f result.txt
	./zad4 txt.txt result.txt sysopy SYSOPY
	./zad4 txt.txt result2.txt sysopy .


clean:
	rm -f zad4 result.txt