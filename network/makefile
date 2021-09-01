echoclient: echoclient.c wrappers.h wrappers.o
	gcc -o echoclient echoclient.c wrappers.o

echoserver: echoserver.c wrappers.h wrappers.o
	gcc -o echoserver echoserver.c wrappers.o

hostinfo: hostinfo.c wrappers.o wrappers.h
	gcc -o hostinfo wrappers.o hostinfo.c

wrappers.o: wrappers.c
	gcc -o wrappers.o -c wrappers.c
