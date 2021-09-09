sharing: sharing.c wrappers.o wrappers.h
	gcc -o sharing -pthread sharing.c wrappers.o

badcnt: badcnt.c wrappers.o wrappers.h
	gcc -o badcnt -pthread badcnt.c wrappers.o

goodcnt: goodcnt.c wrappers.o wrappers.h
	gcc -o goodcnt -pthread goodcnt.c wrappers.o

wrappers.o: wrappers.c
	gcc -o wrappers.o -pthread -c wrappers.c
