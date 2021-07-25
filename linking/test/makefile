prog: main.c -ldl libvector.so
	gcc -rdynamic -o prog main.c -ldl
libvector.so: addvec.c multvec.c
	gcc -shared -fpic -o libvector.so addvec.c multvec.c
