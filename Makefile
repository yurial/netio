.PHONY: all clean install uninstall dist man obj
CFLAGS=-DVERSION="`cat version`" -DBUILD="`cat build`" -DDATE="`cat date`"
-include Makefile.inc
-include $(wildcard %.d)

all: netio

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
DEP=$(SRC:.c=.d)

netio: $(OBJ)
$(OBJ): Makefile Makefile.inc version build date
$(OBJ): $(DEP)
%.d: %.c
	@echo "$@ " > $@
	@cc -MM $< >> $@

clean:
	rm -f *.d *.o netio man/man1

distclean:
	rm -f *.d *.o netio man/man1 *.gz *.bz2

install: netio man
	cp netio /usr/bin
	cp man/man1/netio.1.bz2 /usr/share/man/man1

uninstall:
	rm -f /usr/bin/netio
	rm -f /usr/man/man1/netio.1.bz2

dist: netio.tar.gz netio.tar.bz2

netio.tar.gz: *.c *.h man/* configure makefile.def
	tar --gzip -cf netio.tar.gz *.c *.h man/* configure makefile.def

netio.tar.bz2: *.c *.h man/* configure makefile.def
	tar --bzip2 -cf netio.tar.bz2  *.c *.h man/* configure makefile.def

man: man/man1/netio.1.bz2

man/man1/netio.1.bz2: man/man.1.en
	mkdir -p man/man1
	bzip2 -zkc --best man/man.1.en >man/man1/netio.1.bz2

