CC = gcc -Wall -std=c11 -g

all: zad2 zad2sys

zad2: zad2.c
	$(CC) zad2.c -o zad2

zad2sys: zad2sys.c
	$(CC) zad2sys.c -o zad2sys

test:
	rm -f pomiar_zad_2.txt
	make -s zad2
	echo "\n./zad2 f txt.txt" >> pomiar_zad_2.txt
	./zad2 f txt.txt
	echo "\n./zad2 + txt.txt" >> pomiar_zad_2.txt
	./zad2 + txt.txt
	echo "\n./zad2 ] txt.txt" >> pomiar_zad_2.txt
	./zad2 ] txt.txt
	echo "\n\n\n\n"
	make -s zad2sys
	echo "\n./zad2sys f txt.txt" >> pomiar_zad_2.txt
	./zad2sys f txt.txt
	echo "\n../zad2sys + txt.txt" >> pomiar_zad_2.txt
	./zad2sys + txt.txt
	echo "\n../zad2sys ] txt.txt" >> pomiar_zad_2.txt
	./zad2sys ] txt.txt

clean:
	rm -f zad2 zad2sys