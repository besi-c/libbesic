CC = gcc
CFLAGS = -Wall -std=c17 -O2 -fPIC
prefix = /usr/local


getval.out: getval.c
	$(CC) $(CFLAGS) $^ -static -lbesic -o $@

install: getval.out
	install -T getval.out $(DESTDIR)$(prefix)/bin/besic-getval

clean:
	rm -f *.o *.out
