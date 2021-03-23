CC = gcc -Wall -std=c11 -g

all:
	make zad2_static

static:
	$(CC) -c library.c
	ar rcs library.a library.o

zad2_static:
	make static
	$(CC) -o test zad2.c library.a

main_static:
	make static
	$(CC) -o test2 main.c library.a

clean:
	rm -f *.o *.a test test2 *tmp.txt

test_short_files_small_merges:
	./test  5 short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt

test_short_files_medium_merges:
	./test 25 short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt


test_short_files_big_merges:
	./test 100 short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt


test_medium_files_small_merges:
	./test 5 medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt

test_medium_files_medium_merges:
	./test 25 medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt

test_medium_files_big_merges:
	./test 100 medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt

test_long_files_small_merges:
	./test 5 long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt

test_long_files_medium_merges:
	./test 25 long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt

test_long_files_big_merges:
	./test 100 long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt

tests_for_report:
	rm -f raport.txt
	echo "\n	===short files (4 rows) small merges (5 merges)===" >> raport.txt
	make -s test_short_files_small_merges >> raport.txt
	echo "\n	===short files (4 rows) medium merges (25 merges)===" >> raport.txt
	make -s test_short_files_medium_merges >> raport.txt
	echo "\n	===short files (4 rows) big merges (100 merges) and small block remove and create===" >> raport.txt
	make -s test_short_files_big_merges >> raport.txt
	echo "\n	===medium files (700 rows) small merges (5 merges)===" >> raport.txt
	make -s test_medium_files_small_merges >> raport.txt
	echo "\n	===medium files (700 rows) medium merges (25 merges)===" >> raport.txt
	make -s test_medium_files_medium_merges >> raport.txt
	echo "\n	===medium files (700 rows) big merges (100 merges) and medium block remove and create===" >> raport.txt
	make -s test_medium_files_big_merges >> raport.txt
	echo "\n	===long files (20000 rows) small merges (5 merges)===" >> raport.txt
	make -s test_long_files_small_merges >> raport.txt
	echo "\n	===long files (20000 rows) medium merges (25 merges)===" >> raport.txt
	make -s test_long_files_medium_merges >> raport.txt
	echo "\n	===long files (20000 rows) big merges (100 merges) and big block remove and create===" >> raport.txt
	make -s test_long_files_big_merges >> raport.txt


create_report:
	make -s tests_for_report

test2_short_files_small_merges:
	./test2 create_table 5 merge_file_sequence short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt

test2_short_files_medium_merges:
	./test2 create_table 25 merge_file_sequence short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt


test2_short_files_big_merges:
	./test2 create_table 100 merge_file_sequence short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt


test2_medium_files_small_merges:
	./test2 create_table 5 merge_file_sequence medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt

test2_medium_files_medium_merges:
	./test2 create_table 25 merge_file_sequence medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt

test2_medium_files_big_merges:
	./test2 create_table 100 merge_file_sequence medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt

test2_long_files_small_merges:
	./test2 create_table 5 merge_file_sequence long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt

test2_long_files_medium_merges:
	./test2 create_table 25 merge_file_sequence long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt

test2_long_files_big_merges:
	./test2 create_table 100 merge_file_sequence long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt

tests_for_report2:
	rm -f raport2.txt
	echo "\n	===short files (4 rows) small merges (5 merges)===" >> raport2.txt
	make -s test2_short_files_small_merges >> raport2.txt
	echo "\n	===short files (4 rows) medium merges (25 merges)===" >> raport2.txt
	make -s test2_short_files_medium_merges >> raport2.txt
	echo "\n	===short files (4 rows) big merges (100 merges) and small block remove and create===" >> raport2.txt
	make -s test2_short_files_big_merges >> raport2.txt
	echo "\n	===medium files (700 rows) small merges (5 merges)===" >> raport2.txt
	make -s test2_medium_files_small_merges >> raport2.txt
	echo "\n	===medium files (700 rows) medium merges (25 merges)===" >> raport2.txt
	make -s test2_medium_files_medium_merges >> raport2.txt
	echo "\n	===medium files (700 rows) big merges (100 merges) and medium block remove and create===" >> raport2.txt
	make -s test2_medium_files_big_merges >> raport2.txt
	echo "\n	===long files (20000 rows) small merges (5 merges)===" >> raport2.txt
	make -s test2_long_files_small_merges >> raport2.txt
	echo "\n	===long files (20000 rows) medium merges (25 merges)===" >> raport2.txt
	make -s test2_long_files_medium_merges >> raport2.txt
	echo "\n	===long files (20000 rows) big merges (100 merges) and big block remove and create===" >> raport2.txt
	make -s test2_long_files_big_merges >> raport2.txt

create_report2:
	make -s tests_for_report2