CC = gcc -Wall -std=c11 -g

all:
	make zad2_static

static:
	$(CC) -c library.c
	ar rcs library.a library.o

zad2_static:
	make static
	$(CC) -o test zad2.c library.a

create_report:
	make -s tests_for_report

clean:
	rm -f *.o *.a test *tmp.txt tmp/*.txt

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
	echo "\n	===short files (4 rows) big merges (100 merges)===" >> raport.txt
	make -s test_short_files_big_merges >> raport.txt
	echo "\n	===medium files (700 rows) small merges (5 merges)===" >> raport.txt
	make -s test_medium_files_small_merges >> raport.txt
	echo "\n	===medium files (700 rows) medium merges (25 merges)===" >> raport.txt
	make -s test_medium_files_medium_merges >> raport.txt
	echo "\n	===medium files (700 rows) big merges (100 merges)===" >> raport.txt
	make -s test_medium_files_big_merges >> raport.txt
	echo "\n	===long files (50000 rows) small merges (5 merges)===" >> raport.txt
	make -s test_long_files_small_merges >> raport.txt
	echo "\n	===long files (50000 rows) medium merges (25 merges)===" >> raport.txt
	make -s test_long_files_medium_merges >> raport.txt
	echo "\n	===long files (50000 rows) big merges (100 merges)===" >> raport.txt
	make -s test_long_files_big_merges >> raport.txt


