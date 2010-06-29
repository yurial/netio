OPT=-g

netio: netio.o help.o params.o mkserver.o mkclient.o net.o cp.o mainloop.o set.o run.o collision.o makefile
	gcc netio.o help.o params.o mkserver.o mkclient.o net.o cp.o mainloop.o set.o run.o collision.o -o netio

clean:
	rm *.o netio

netio.o: netio.c netio.h
	gcc $(OPT) -c netio.c -o netio.o

help.o: help.c help.h
	gcc $(OPT) -c help.c -o help.o

params.o: params.c params.h
	gcc $(OPT) -c params.c -o params.o

mkserver.o: mkserver.c mkserver.h
	gcc $(OPT) -c mkserver.c -o mkserver.o

mkclient.o: mkclient.c mkclient.h
	gcc $(OPT) -c mkclient.c -o mkclient.o

net.o: net.c net.h
	gcc $(OPT) -c net.c -o net.o

cp.o: cp.c cp.h
	gcc $(OPT) -c cp.c -o cp.o

mainloop.o: mainloop.c mainloop.h
	gcc $(OPT) -c mainloop.c -o mainloop.o

set.o: set.c set.h
	gcc $(OPT) -c set.c -o set.o

run.o: run.c run.h
	gcc $(OPT) -c run.c -o run.o

collision.o: collision.c collision.h
	gcc $(OPT) -c collision.c -o collision.o
