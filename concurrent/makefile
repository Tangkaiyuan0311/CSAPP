all: echoclient echoserverp echoservers echoservert echoserver_pre

echoserver_pre: echoserver_pre.c sbuf.c sbuf.h echo_cnt.c wrappers.o
	gcc -o echoserver_pre -pthread echoserver_pre.c sbuf.c echo_cnt.c wrappers.o

echoclient: echoclient.c wrappers.o wrappers.h
	gcc -o echoclient -pthread echoclient.c wrappers.o

echoserverp: echoserverp.c wrappers.o wrappers.h
	gcc -o echoserverp -pthread echoserverp.c wrappers.o

echoservers: echoservers.c wrappers.o wrappers.h
	gcc -o echoservers -pthread echoservers.c wrappers.o

echoservert: echoservert.c wrappers.o wrappers.h
	gcc -o echoservert -pthread echoservert.c wrappers.o

wrappers.o: wrappers.c wrappers.h
	gcc -o wrappers.o -pthread -c wrappers.c
