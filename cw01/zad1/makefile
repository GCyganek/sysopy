CC = gcc -Wall -std=c11 -g

static:
	$(CC) -c library.c
	ar rcs library.a library.o

shared:
	$(CC) -fPIC -c library.c
	$(CC) -shared -fPIC -o library.so library.o

all:
	make static shared

clean:
	rm -f *.o *.a *.so *.out