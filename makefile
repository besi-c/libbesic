CC = gcc
CFLAGS = -g -Wall -std=c17 -O3 -fPIC
SOVERSION = 2

prefix = /usr/local
bindir = $(prefix)/bin
includedir = $(prefix)/include
libdir = $(prefix)/lib
etcdir = /etc/besic

all: lib getval.out heartbeat.out secret.out confgen.out

lib: libbesic.so


.PHONY: test
test: test.out
	LD_LIBRARY_PATH="$$LD_LIBRARY_PATH:." ./test.out


install: all
	install -m 0755 -d $(DESTDIR)$(includedir)
	install -m 0644 besic.h $(DESTDIR)$(includedir)
	install -m 0755 -d $(DESTDIR)$(libdir)
	install -m 0755 libbesic.so $(DESTDIR)$(libdir)/libbesic.so.$(SOVERSION)
	cd $(DESTDIR)$(libdir) && ln -fs libbesic.so.$(SOVERSION) libbesic.so
	install -m 0755 -d $(DESTDIR)$(bindir)
	install -m 0755 getval.out $(DESTDIR)$(bindir)/besic-getval
	install -m 0755 heartbeat.out $(DESTDIR)$(bindir)/besic-heartbeat
	install -m 0755 secret.out $(DESTDIR)$(bindir)/besic-secret
	install -m 0755 -d $(DESTDIR)$(etcdir)
	LD_LIBRARY_PATH="$$LD_LIBRARY_PATH:." ./confgen.out > $(DESTDIR)$(etcdir)/besic.conf

remove:
	rm -f $(DESTDIR)$(includedir)/besic.h
	rm -f $(DESTDIR)$(libdir)/libbesic.*
	rm -f $(DESTDIR)$(bindir)/besic-getval
	rm -f $(DESTDIR)$(bindir)/besic-heartbeat
	rm -f $(DESTDIR)$(bindir)/besic-secret


getval.out: getval.c libbesic.so
	$(CC) $(CFLAGS) -pie $^ -o $@

heartbeat.out: heartbeat.c libbesic.so
	$(CC) $(CFLAGS) -pie $^ -o $@

secret.out: secret.c libbesic.so
	$(CC) $(CFLAGS) -pie $^ -o $@

confgen.out: confgen.c libbesic.so
	$(CC) $(CFLAGS) -pie $^ -o $@

test.out: test.c libbesic.so
	$(CC) $(CFLAGS) -pie $^ -o $@


libbesic.so: besic.o
	$(CC) $(CFLAGS) -shared -Wl,-soname,$@.$(SOVERSION) -fPIE $^ -lcurl -o $@
	ln -fs $@ $@.$(SOVERSION)

libbesic.a: besic.o
	$(AR) rcs $@ $^

besic.o: besic.c besic.h


clean:
	rm -f *.o *.out *.so *.so.* *.a
