OPT ?= -O0
CC = gcc -Wall -std=c11 -g $(OPT)

all:
	make -s create_report

static:
	$(CC) -c library.c
	ar rcs library.a library.o

shared:
	$(CC) -fPIC -c library.c
	$(CC) -shared -fPIC -o liblibrary.so library.o

main_static:
	make static
	$(CC) -o test main.c library.a

main_shared:
	make shared
	$(CC) -Wl,-rpath=. -o test main.c liblibrary.so

main_dynamic:
	make shared
	$(CC) -o test main_dynamic.c -ldl -D DYNAMIC

clean:
	rm -f *.o *.a *.so test tmp

test_short_files_small_merges:
	./test create_table 5 merge_file_sequence short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt

test_short_files_medium_merges:
	./test create_table 25 merge_file_sequence short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt


test_short_files_big_merges:
	./test create_table 100 merge_file_sequence short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
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
	short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt short1.txt:short2.txt \
	delete_block 0 create_block


test_medium_files_small_merges:
	./test create_table 5 merge_file_sequence medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt

test_medium_files_medium_merges:
	./test create_table 25 merge_file_sequence medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt

test_medium_files_big_merges:
	./test create_table 100 merge_file_sequence medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
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
	medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt medium1.txt:medium2.txt \
	delete_block 0 create_block

test_long_files_small_merges:
	./test create_table 5 merge_file_sequence long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt

test_long_files_medium_merges:
	./test create_table 25 merge_file_sequence long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt

test_long_files_big_merges:
	./test create_table 100 merge_file_sequence long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
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
	long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt long1.txt:long2.txt \
	delete_block 0 create_block

test_create_and_remove_blocks:
	./test create_table 300 write_files_to_tmp long1.txt long2.txt create_and_remove_blocks 100 create_and_remove_blocks 200 create_and_remove_blocks 300

tests_for_report:
	echo "\n	===short files (4 rows) small merges (5 merges)===" >> raport3b.txt
	make -s test_short_files_small_merges >> raport3b.txt
	echo "\n	===short files (4 rows) small merges (5 merges)==="
	make -s test_short_files_small_merges
	echo "\n	===short files (4 rows) medium merges (25 merges)===" >> raport3b.txt
	make -s test_short_files_medium_merges >> raport3b.txt
	echo "\n	===short files (4 rows) medium merges (25 merges)==="
	make -s test_short_files_medium_merges
	echo "\n	===short files (4 rows) big merges (100 merges) and small block remove and create===" >> raport3b.txt
	make -s test_short_files_big_merges >> raport3b.txt
	echo "\n	===short files (4 rows) big merges (100 merges) and small block remove and create==="
	make -s test_short_files_big_merges
	echo "\n	===medium files (700 rows) small merges (5 merges)===" >> raport3b.txt
	make -s test_medium_files_small_merges >> raport3b.txt
	echo "\n	===medium files (700 rows) small merges (5 merges)==="
	make -s test_medium_files_small_merges
	echo "\n	===medium files (700 rows) medium merges (25 merges)===" >> raport3b.txt
	make -s test_medium_files_medium_merges >> raport3b.txt
	echo "\n	===medium files (700 rows) medium merges (25 merges)==="
	make -s test_medium_files_medium_merges
	echo "\n	===medium files (700 rows) big merges (100 merges) and medium block remove and create===" >> raport3b.txt
	make -s test_medium_files_big_merges >> raport3b.txt
	echo "\n	===medium files (700 rows) big merges (100 merges) and medium block remove and create==="
	make -s test_medium_files_big_merges
	echo "\n	===long files (2000 rows) small merges (5 merges)===" >> raport3b.txt
	make -s test_long_files_small_merges >> raport3b.txt
	echo "\n	===long files (2000 rows) small merges (5 merges)==="
	make -s test_long_files_small_merges
	echo "\n	===long files (2000 rows) medium merges (25 merges)===" >> raport3b.txt
	make -s test_long_files_medium_merges >> raport3b.txt
	echo "\n	===long files (2000 rows) medium merges (25 merges)==="
	make -s test_long_files_medium_merges
	echo "\n	===long files (2000 rows) big merges (100 merges) and big block remove and create===" >> raport3b.txt
	make -s test_long_files_big_merges >> raport3b.txt
	echo "\n	===long files (2000 rows) big merges (100 merges), big block remove and create==="
	make -s test_long_files_big_merges
	echo "\n	===creating and deleting 100, 200 and 300 blocks from tmp file merged from long files (2000 rows)===" >> raport3b.txt
	make -s test_create_and_remove_blocks >> raport3b.txt
	echo "\n	===creating and deleting 100, 200 and 300 blocks from tmp file merged from long files (2000 rows)==="
	make -s test_create_and_remove_blocks

create_report:
	rm -f raport3b.txt
	make clean
	make -s main_static OPT=-O0
	echo "\n\n\n\n\n*****STATIC $(OPT)*****" >> raport3b.txt
	echo "\n\n\n\n\n*****STATIC $(OPT)*****"
	make -s tests_for_report
	make -s main_shared OPT=-O0
	echo "\n\n\n\n\n*****SHARED $(OPT)*****" >> raport3b.txt
	echo "\n\n\n\n\n*****SHARED $(OPT)*****"
	make -s tests_for_report
	make -s main_dynamic OPT=-O0
	echo "\n\n\n\n\n*****DYNAMIC $(OPT)*****" >> raport3b.txt
	echo "\n\n\n\n\n*****DYNAMIC $(OPT)*****"
	make -s tests_for_report
	make clean

	make -s main_static OPT=-O3
	echo "\n\n\n\n\n*****STATIC $(OPT)*****" >> raport3b.txt
	echo "\n\n\n\n\n*****STATIC $(OPT)*****"
	make -s tests_for_report
	make -s main_shared OPT=-O3
	echo "\n\n\n\n\n*****SHARED $(OPT)*****" >> raport3b.txt
	echo "\n\n\n\n\n*****SHARED $(OPT)*****"
	make -s tests_for_report
	make -s main_dynamic OPT=-O3
	echo "\n\n\n\n\n*****DYNAMIC $(OPT)*****" >> raport3b.txt
	echo "\n\n\n\n\n*****DYNAMIC $(OPT)*****"
	make -s tests_for_report
	make clean

	make -s main_static OPT=-Os
	echo "\n\n\n\n\n*****STATIC $(OPT)*****" >> raport3b.txt
	echo "\n\n\n\n\n*****STATIC $(OPT)*****"
	make -s tests_for_report
	make -s main_shared OPT=-Os
	echo "\n\n\n\n\n*****SHARED $(OPT)*****" >> raport3b.txt
	echo "\n\n\n\n\n*****SHARED $(OPT)*****"
	make -s tests_for_report
	make -s main_dynamic OPT=-Os
	echo "\n\n\n\n\n*****DYNAMIC $(OPT)*****" >> raport3b.txt
	echo "\n\n\n\n\n*****DYNAMIC $(OPT)*****"
	make -s tests_for_report


