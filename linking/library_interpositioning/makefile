#./main_compiletime
main_compiletime: main.c mymalloc_compiletime.o malloc.h #include local header file due to -I. option
	gcc -I. -o main_compiletime main.c mymalloc_compiletime.o

mymalloc_compiletime.o: mymalloc.c
	gcc -DCOMPILETIME -o mymalloc_compiletime.o -c mymalloc.c






#./main_runtime
main_linktime: main.c mymalloc_linktime.o malloc.h #include system header file
	gcc -Wl,--wrap,malloc -Wl,--wrap,free -o main_linktime main.c mymalloc_linktime.o

mymalloc_linktime.o: mymalloc.c
	gcc -DLINKTIME -o mymalloc_linktime.o -c mymalloc.c







#LD_PRELOAD="./mymalloc.so" ./main_runtime
main_runtime: main.c mymalloc.so malloc.h
	gcc -o main_runtime main.c

mymalloc.so: mymalloc.c -ldl
	gcc -DRUNTIME -shared -fpic -o mymalloc.so mymalloc.c -ldl

