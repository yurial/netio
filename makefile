netio: netio.o help.o params.o mkserver.o mkclient.o net.o cp.o mainloop.o terminal.o set.o
	gcc netio.o help.o params.o mkserver.o mkclient.o net.o cp.o mainloop.o terminal.o set.o -o netio

clean:
	rm *.o netio

netio.o: netio.c netio.h
	gcc -c netio.c -o netio.o

help.o: help.c help.h
	gcc -c help.c -o help.o

params.o: params.c params.h
	gcc -DDEBUG -c params.c -o params.o

mkserver.o: mkserver.c mkserver.h
	gcc -c mkserver.c -o mkserver.o

mkclient.o: mkclient.c mkclient.h
	gcc -c mkclient.c -o mkclient.o

net.o: net.c net.h
	gcc -c net.c -o net.o

cp.o: cp.c cp.h
	gcc -c cp.c -o cp.o

mainloop.o: mainloop.c mainloop.h
	gcc -c mainloop.c -o mainloop.o

terminal.o: terminal.c terminal.h
	gcc -c terminal.c -o terminal.o

set.o: set.c set.h
	gcc -c set.c -o set.o

