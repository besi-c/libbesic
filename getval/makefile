CC = clang
CFLAGS = -Wall -std=c17 -O2 -fPIC
prefix = /usr/local


getval.out: getval.c
	$(CC) $(CFLAGS) -lbesic $^ -o $@

install: getval.out
	install -T getval.out $(DESTDIR)$(prefix)/bin/besic-getval

clean:
	rm -f *.o *.out
