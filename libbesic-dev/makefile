CC = clang
CFLAGS = -Wall -std=c17 -O2 -fPIC
prefix = /usr/local


lib: libbesic.so libbesic.a

.PHONY: test
test: test.out
	./test.out


test.out: test.c besic.o
	$(CC) $(CFLAGS) $^ -o $@

libbesic.so: besic.o
	$(CC) $(CFLAGS) -shared $^ -o $@

libbesic.a: besic.o
	$(AR) rs $@ $^

besic.o: besic.c besic.h


install: lib
	install libbesic.so $(DESTDIR)$(prefix)/lib
	install libbesic.a $(DESTDIR)$(prefix)/lib
	install besic.h $(DESTDIR)$(prefix)/include

clean:
	rm -f *.o *.out *.so *.a
