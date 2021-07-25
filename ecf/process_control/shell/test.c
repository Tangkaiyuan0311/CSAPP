#include <stdio.h>


char* Fgets(char*, int, FILE*);

int main(){
	char buffer[20];
	Fgets(buffer, 10, stdin);
	printf("read in: %s\n", buffer);
	return 0;
}
