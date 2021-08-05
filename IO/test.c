#include "wrappers.h"

int main() {
	char buf[20];
	printf("Promise to read/consume 10 bytes from terminal(includinf EOL character)...\n");
	ssize_t nread = rio_readn(STDIN_FILENO, buf, 10);
	printf("read %ld bytes, write them to terminal\n", nread);
	ssize_t nwritten = rio_writen(STDOUT_FILENO, buf, 10);
	printf("\nwrite %ld bytes to terminal\n", nwritten);
	return 0;
}
