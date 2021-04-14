CC = gcc -Wall -std=c11 -g -D_XOPEN_SOURCE=700
N := 5

all: main consumer producer check

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
	make -s generate_txt
	make -s N=5 test_5_prods_1_cons
	make -s show_result_and_delete
	make -s N=5 test_5_prods_5_cons
	make -s show_result_and_delete
	make -s N=5 test_1_prods_5_cons
	make -s show_result_and_delete
	make -s N=10 test_5_prods_1_cons
	make -s show_result_and_delete
	make -s N=10 test_5_prods_5_cons
	make -s show_result_and_delete
	make -s N=10 test_1_prods_5_cons
	make -s show_result_and_delete
	make -s N=25 test_5_prods_1_cons
	make -s show_result_and_delete
	make -s N=25 test_5_prods_5_cons
	make -s show_result_and_delete
	make -s N=25 test_1_prods_5_cons
	make -s show_result_and_delete
	make -s test_in_different_terminals

show_result_and_delete:
	cat result.txt
	rm result.txt
	rm fifo

test_5_prods_1_cons:
	./main $(N) # 5 producers - 1 consumer
	make check_for_5_producers
	echo -n "\nfor ./main N 4 one_producer # 5 producers - 1 consumer\n" >> result.txt

test_5_prods_5_cons:
	./main $(N) 4 # 5 producers - 5 consumers
	make check_for_5_producers
	echo "\nfor ./main N 4 one_producer # 5 producers - 5 consumers\n\n" >> result.txt

test_1_prods_5_cons:
	./main $(N) 4 one_producer # 5 producers - 5 consumers
	./check prod_1.txt result.txt 1
	echo "\nfor ./main N 4 one_producer # 5 producers - 5 consumers\n\n" >> result.txt

check_for_5_producers:
	./check prod_1.txt result.txt 1
	./check prod_2.txt result.txt 2
	./check prod_3.txt result.txt 3
	./check prod_4.txt result.txt 4
	./check prod_5.txt result.txt 5


test_in_different_terminals:
	mkfifo "fifo"
	gnome-terminal -e "bash -c \"./consumer fifo result.txt 5; exec bash\""
	gnome-terminal -e "bash -c \"./consumer fifo result.txt 5; exec bash\""
	gnome-terminal -e "bash -c \"./consumer fifo result.txt 5; exec bash\""
	gnome-terminal -e "bash -c \"./consumer fifo result.txt 5; exec bash\""
	gnome-terminal -e "bash -c \"./consumer fifo result.txt 5; exec bash\""
	gnome-terminal -e "bash -c \"./producer fifo 1 prod_1.txt 5; exec bash\""
	gnome-terminal -e "bash -c \"./producer fifo 2 prod_2.txt 5; exec bash\""
	gnome-terminal -e "bash -c \"./producer fifo 3 prod_3.txt 5; exec bash\""
	gnome-terminal -e "bash -c \"./producer fifo 4 prod_4.txt 5; exec bash\""
	gnome-terminal -e "bash -c \"./producer fifo 5 prod_5.txt 5; exec bash\""

clean:
	rm -f main producer consumer *.txt fifo check