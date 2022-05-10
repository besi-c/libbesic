CC = gcc
CFLAGS = -g -Wall -std=c17 -O3 -fPIC
SOVERSION = 2

python_version := $(wordlist 2,4,$(subst ., ,$(shell python --version)))
PY_MAJOR := $(word 1,${python_version})
PY_MINOR := $(word 2,${python_version})

prefix = /usr/local
bindir = $(prefix)/bin
includedir = $(prefix)/include
libdir = $(prefix)/lib
etcdir = /etc/besic



all: lib getval.out heartbeat.out secret.out confgen.out

lib: libbesic.so besic.cpython.so


.PHONY: test clean
test: test.out
	LD_LIBRARY_PATH="$$LD_LIBRARY_PATH:." ./test.out

clean:
	rm -rf src/*.o src/*.py.c *.out *.so *.so.* *.a


# Intermediate files
src/besic.o: src/besic.c src/besic.h
	$(CC) $(CFLAGS) -c $< -o $@

src/besic.py.c: src/besic.pyx
	python3 -m cython -3 $^ -o $@

src/besic.py.o: src/besic.py.c
	$(CC) $(CFLAGS) -I /usr/include/python$(PY_MAJOR).$(PY_MINOR) -c $< -o $@


# Libraries
libbesic.so: src/besic.o
	$(CC) $(CFLAGS) -shared -Wl,-soname,$@.$(SOVERSION) -fPIE $^ -lcurl -o $@
	ln -fs $@ $@.$(SOVERSION)

libbesic.a: src/besic.o
	$(AR) rcs $@ $^

besic.cpython.so: src/besic.py.o
	$(CC) $(CFLAGS) -L. -shared -Wl,-soname,besic.py -fPIE $^ -lbesic -o $@


# Executables
getval.out: src/getval.c libbesic.so
	$(CC) $(CFLAGS) -pie $^ -o $@

heartbeat.out: src/heartbeat.c libbesic.so
	$(CC) $(CFLAGS) -pie $^ -o $@

secret.out: src/secret.c libbesic.so
	$(CC) $(CFLAGS) -pie $^ -o $@

confgen.out: src/confgen.c libbesic.so
	$(CC) $(CFLAGS) -pie $^ -o $@

test.out: src/test.c libbesic.so
	$(CC) $(CFLAGS) -pie $^ -o $@



# Installation
install: all
	install -m 0755 -d $(DESTDIR)$(includedir)
	install -m 0644 src/besic.h $(DESTDIR)$(includedir)
	install -m 0755 -d $(DESTDIR)$(libdir)
	install -m 0755 libbesic.so $(DESTDIR)$(libdir)/libbesic.so.$(SOVERSION)
	cd $(DESTDIR)$(libdir) && ln -fs libbesic.so.$(SOVERSION) libbesic.so
	install -m 0755 -d $(DESTDIR)$(bindir)
	install -m 0755 getval.out $(DESTDIR)$(bindir)/besic-getval
	install -m 0755 heartbeat.out $(DESTDIR)$(bindir)/besic-heartbeat
	install -m 0755 secret.out $(DESTDIR)$(bindir)/besic-secret
	install -m 0755 -d $(DESTDIR)$(etcdir)
	LD_LIBRARY_PATH="$$LD_LIBRARY_PATH:." ./confgen.out > $(DESTDIR)$(etcdir)/besic.conf
	install -m 0755 -d $(DESTDIR)$(libdir)/python$(PY_MAJOR).$(PY_MINOR)
	install -m 0644 besic.cpython.so $(DESTDIR)$(libdir)/python$(PY_MAJOR).$(PY_MINOR)/besic.cpython-$(PY_MAJOR)$(PY_MINOR)-$$(gcc -dumpmachine).so

remove:
	rm -f $(DESTDIR)$(includedir)/besic.h
	rm -f $(DESTDIR)$(libdir)/libbesic.*
	rm -f $(DESTDIR)$(bindir)/besic-getval
	rm -f $(DESTDIR)$(bindir)/besic-heartbeat
	rm -f $(DESTDIR)$(bindir)/besic-secret
	rm -f $(DESTDIR)$(libdir)/python$(PY_MAJOR).$(PY_MINOR)/besic.*.so
