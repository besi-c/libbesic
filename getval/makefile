CFLAGS = -Wall -std=c17 -O2 -fPIE -g
prefix = /usr/local


getval.out: getval.c
	$(CC) $(CFLAGS) $^ -pie -lbesic -o $@

install: getval.out
	mkdir -p $(DESTDIR)$(prefix)/bin
	install -T getval.out $(DESTDIR)$(prefix)/bin/besic-getval

clean:
	rm -f *.o *.out
