all: echoclient echoserverp echoservers

echoclient: echoclient.c wrappers.o wrappers.h
	gcc -o echoclient echoclient.c wrappers.o

echoserverp: echoserverp.c wrappers.o wrappers.h
	gcc -o echoserverp echoserverp.c wrappers.o

echoservers: echoservers.c wrappers.o wrappers.h
	gcc -o echoservers echoservers.c wrappers.o

wrappers.o: wrappers.c wrappers.h
	gcc -o wrappers.o -c wrappers.c
