waitpid: waitpid.c wrappers.o
	gcc -o waitpid waitpid.c wrappers.o

waitpid_order: waitpid.c wrappers.o
	gcc -DORDER -o waitpid_order waitpid.c wrappers.o

wrappers.o: wrappers.c
	cc -o wrappers.o -c wrappers.c
