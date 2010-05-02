netio: netio.o help.o params.o doserver.o doclient.o
	gcc netio.o help.o params.o doserver.o doclient.o -o netio

netio.o: netio.c netio.h
	gcc -c netio.c -o netio.o

help.o: help.c help.h
	gcc -c help.c -o help.o

params.o: params.c params.h
	gcc -DDEBUG -c params.c -o params.o

doserver.o: doserver.c doserver.h
	gcc -c doserver.c -o doserver.o

doclient.o: doclient.c doclient.h
	gcc -c doclient.c -o doclient.o
