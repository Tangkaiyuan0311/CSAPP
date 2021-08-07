#include "wrappers.h"
#define MAXLINE 8192

int main(){
	char buf[100];
	rio_t rio;
	rio_readinitb(&rio, STDIN_FILENO);
	int fd = Open("test.txt", O_RDONLY, 0);
	int ret = Dup2(fd, STDIN_FILENO);
	printf("dup2 return fd = %d\n", ret);
	int n;
	while ((n = rio_readlineb(&rio, buf, MAXLINE)) != 0)
		rio_writen(STDOUT_FILENO, buf, n);
	Close(fd);
	exit(0);
}
