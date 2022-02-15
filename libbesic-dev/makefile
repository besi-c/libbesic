CC = gcc
CFLAGS = -Wall -std=c17 -O2 -fPIC
prefix = /usr/local


lib: libbesic.a

.PHONY: test
test: test.out
	./test.out

install: lib
	mkdir -p $(DESTDIR)$(prefix)/lib
	mkdir -p $(DESTDIR)$(prefix)/include
	cp libbesic.a $(DESTDIR)$(prefix)/lib
	cp besic.h $(DESTDIR)$(prefix)/include


test.out: test.c besic.o
	$(CC) $(CFLAGS) $^ -o $@

libbesic.so: besic.o
	$(CC) $(CFLAGS) -shared $^ -o $@

libbesic.a: besic.o
	$(AR) rcs $@ $^

besic.o: besic.c besic.h


clean:
	rm -f *.o *.out *.so *.a
