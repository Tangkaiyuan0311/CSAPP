IO_redir: IO_redir.c wrappers.o wrappers.h
	gcc -o IO_redir IO_redir.c wrappers.o

readdir: readdir.c wrappers.o wrappers.h
	gcc -o readdir readdir.c wrappers.o

file: file.c wrappers.o wrappers.h
	gcc -o file file.c wrappers.o

rio: rio.c wrappers.o wrappers.h
	gcc -o rio rio.c wrappers.o

wrappers.o: wrappers.c
	gcc -o wrappers.o -c wrappers.c

all: IO_redir readdir file rio
