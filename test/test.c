#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
int main(void) {
	char c;
	while (read(STDIN_FILENO, &c, 1) != 0) {
		if (c != '\n')
			write(STDOUT_FILENO, &c, 1);
	}
	exit(0);
}
