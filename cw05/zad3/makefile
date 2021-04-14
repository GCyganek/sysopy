CC = gcc -Wall -std=c11 -g -D_XOPEN_SOURCE=700

all: main consumer producer

check: check.c

consumer: consumer.c
	$(CC) consumer.c -o consumer

producer: producer.c
	$(CC) producer.c -o producer

main: main.c
	$(CC) main.c -o main

check: check.c
	$(CC) check.c -o check

generate_txt:
	rm -f *.txt
	echo -n "12345678901234567890123456789012345678901234567890" >> prod_1.txt
	echo -n "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb" >> prod_2.txt
	echo -n "cccccccccccccccccccccccccccccccccccccccccccccccccc" >> prod_3.txt
	echo -n "dddddddddddddddddddddddddddddddddddddddddddddddddd" >> prod_4.txt
	echo -n "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" >> prod_5.txt

test: all
	make generate_txt
	./main 5 # N = 5, 5 producers - 1 consumer
	echo "\nfor ./main 5 4 one_producer # N = 5, 1 producer - 5 consumers\n\n" >> result.txt
	cat result.txt
	rm result.txt
	rm fifo
	./main 5 4 # N = 5, 5 producers - 5 consumers
	echo "\nfor ./main 5 4 one_producer # N = 5, 1 producer - 5 consumers\n\n" >> result.txt
	cat result.txt
	rm result.txt
	rm fifo
	./main 5 4 one_producer # N = 5, 1 producer - 5 consumers
	echo "\nfor ./main 5 4 one_producer # N = 5, 1 producer - 5 consumers\n\n" >> result.txt
	cat result.txt

clean:
	rm -f main producer consumer *.txt fifo check