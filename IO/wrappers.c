#include "wrappers.h"

void unix_error(char* msg){
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
}

int Open(char* filename, int flags, mode_t mode) {
	int fd;
	if ((fd = open(filename, flags, mode)) < 0)
		unix_error("open file error");
	return fd;
}

int Close(int fd) {
	int ret;
    if ((ret = close(fd)) < 0)
        unix_error("close file error");
    return ret;
}

