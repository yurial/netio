CC=gcc
all: netio
.PHONY: all clean install uninstall dist man obj depend
CFLAGS=-DVERSION="\"`cat version`\"" -DBUILD="\"`cat build`\"" -DDATE="\"`cat date`\"" -Wall
-include Makefile.inc
-include *.d

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
DEP=$(SRC:.c=.d)

$(OBJ): Makefile Makefile.inc version build date

%.d: %.c
	echo -n "$@ " > $@ && $(CC) -MM $< >> $@ || rm $@

netio: $(DEP) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

depend: $(DEP)

clean:
	rm -rf *.d *.o netio man/man1

distclean:
	rm -rf *.d *.o netio man/man1 *.gz *.bz2

install: netio man
	cp netio /usr/bin
	cp man/man1/netio.1.bz2 /usr/share/man/man1

symlinks: netio man
	ln -sf $(PWD)/netio /usr/bin/
	ln -sf $(PWD)/man/man1/netio.1.bz2 /usr/share/man/man1/

uninstall:
	rm -f /usr/bin/netio
	rm -f /usr/share/man/man1/netio.1.bz2

dist: netio.tar.gz netio.tar.bz2

netio.tar.gz: *.c *.h man/* configure makefile.def
	tar --gzip -cf netio.tar.gz *.c *.h man/* configure makefile.def

netio.tar.bz2: *.c *.h man/* configure makefile.def
	tar --bzip2 -cf netio.tar.bz2  *.c *.h man/* configure makefile.def

man: man/man1/netio.1.bz2

man/man1/netio.1.bz2: man/man.1.en
	mkdir -p man/man1
	bzip2 -zkc --best man/man.1.en >man/man1/netio.1.bz2

