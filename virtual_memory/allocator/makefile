main: main.c mm.c memlib.c wrappers.o mm.h memlib.h
	gcc -o main main.c memlib.c mm.c wrappers.o

wrappers.o: wrappers.c
	gcc -o wrappers.o -c wrappers.c
