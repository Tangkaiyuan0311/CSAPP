local_jmp: local_jmp.c wrappers.h wrappers.o
	gcc -o local_jmp local_jmp.c wrappers.o

sigsuspend: sigsuspend.c wrappers.h wrappers.o
	gcc -o sigsuspend sigsuspend.c wrappers.o

procmask: procmask.c wrappers.h sig_handlers.c wrappers.o
	gcc -o procmask procmask.c sig_handlers.c wrappers.o

parent: parent.c wrappers.h sig_handlers.c wrappers.o
	gcc -o parent parent.c sig_handlers.c wrappers.o

sig: sig.c wrappers.o wrappers.h
	gcc -o sig sig.c wrappers.o

kill: kill.c wrappers.h wrappers.o
	gcc -o kill kill.c wrappers.o

block: block.c wrappers.h wrappers.o
	gcc -o block block.c wrappers.o

wrappers.o: wrappers.c
	cc -o wrappers.o -c wrappers.c
