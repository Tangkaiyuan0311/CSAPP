#include "wrappers.h"

void unix_error(char* msg){
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
}

FILE* Fopen(const char *restrict pathname, const char *restrict mode){
	FILE* fp;
	if((fp = fopen(pathname, mode)) == NULL)
		unix_error("fopen error");
	return fp;
}

void* Mmap(void* start, size_t length, int prot, int flags, int fd, off_t offset){
	void* areap;
	if((areap = mmap(start, length, prot, flags, fd, offset)) == MAP_FAILED)
		unix_error("mmap error");
	return areap;
}

ssize_t Write(int fd, const void *buf, size_t count){
	ssize_t ret;
	if((ret = write(fd, buf, count)) == -1)
		unix_error("write error");
	return ret;
}

