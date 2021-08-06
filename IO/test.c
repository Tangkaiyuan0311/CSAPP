#include "wrappers.h"
#define MAXLINE 100

int main() {
	char buf[100];
	/*
	printf("Promise to read/consume 10 bytes from terminal(includinf EOL character)...\n");
	ssize_t nread = rio_readn(STDIN_FILENO, buf, 10);
	printf("read %ld bytes, write them to terminal\n", nread);
	ssize_t nwritten = rio_writen(STDOUT_FILENO, buf, 10);
	printf("\nwrite %ld bytes to terminal\n", nwritten);
	*/

	rio_t rio; // buffer struct
	rio_readinitb(&rio, STDIN_FILENO); // assocaited with standard input

	printf("Promise to read/consume 10 bytes from terminal(includinf EOL character)...\n");
    ssize_t nread = rio_readnb(&rio, buf, 10);
    printf("read %ld bytes, write them to terminal\n", nread);
    ssize_t nwritten = rio_writen(STDOUT_FILENO, buf, 10);
    printf("\nwrite %ld bytes to terminal\n", nwritten);
	
	printf("reset user buffer...\n");
	memset(buf, 0, 100);

	int fd = Open("test.txt", O_RDONLY, 0);
	
	rio_readinitb(&rio, fd); // assocaited with test.txt
	int n;
	while ((n = rio_readlineb(&rio, buf, MAXLINE)) != 0)
		rio_writen(STDOUT_FILENO, buf, n);

	Close(fd);
	return 0;
}
