main: main.o
	gcc -o main main.o

main.o: main.s
	as -o main.o main.s

main.s: main.c
	gcc -Og -S main.c
