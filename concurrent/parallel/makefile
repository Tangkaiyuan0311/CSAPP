psum_mutex: psum_mutex.c wrappers.o wrappers.h
	gcc -o psum_mutex -pthread psum_mutex.c wrappers.o

psum_local: psum_local.c wrappers.o wrappers.h
	gcc -o psum_local -pthread psum_local.c wrappers.o

wrappers.o: wrappers.c wrappers.h
	gcc -o wrappers.o -pthread -c wrappers.c
