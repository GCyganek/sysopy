CC = gcc -Wall -std=c11 -g -D_XOPEN_SOURCE=700

all: static catcher sender

static: header.c
	$(CC) -c header.c
	ar rcs header.a header.o

catcher: catcher.c header.a
	$(CC) catcher.c header.a -o catcher

sender: sender.c header.a
	$(CC) sender.c header.a -o sender

test:
	./catcher kill&
	./sender `pidof catcher` 100 kill

	./catcher sigqueue&
	./sender `pidof catcher` 100 sigqueue

	./catcher sigrt&
	./sender `pidof catcher` 100 sigrt

clean:
	rm -f *.o *.a catcher sender