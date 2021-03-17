CC = gcc -Wall -std=c11 -g

all: zad2

zad2: zad2.c
	$(CC) zad2.c -o zad2

test:
	make -s zad2
	./zad2 f txt.txt
	./zad2 + txt.txt
	./zad2 ] txt.txt

clean:
	rm -f zad2