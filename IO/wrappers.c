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

// return: -1 on unrecoverable error, < n on EOF, n for all othercase
ssize_t rio_readn(int fd, void* usrbuf, size_t n) {
	size_t nleft = n; //initialization
	ssize_t nread; // number of bytes read on each call to read, 0 on EOF, -1 on error
	char* bufp = usrbuf;

	while (nleft > 0) { // request is not satisfied yet
		if ((nread = read(fd, bufp, nleft)) < 0) { // read error
			if (errno = EINTR) // read is interrupted by signal handler return
				nread = 0; // perpare for next read
			else
				return -1; // unrecoverable error, return -1
		}
		else if (nread == 0) //EOF, break the read loop and return
			break;
		// update state variable
		nleft -= nread;
		bufp += nread;
	}
	return (n - nleft); // return bytes <= n, less than n only on EOF
}

// return: -1 on unrecoverable error, n otherwise
ssize_t rio_writen(int fd, void* usrbuf, size_t n) {
	size_t nleft = n;
	ssize_t nwritten; // number of bytes read on each call to write, -1 on error
	char* bufp = usrbuf;

	while (nleft > 0) { // remaining bytes to be written to file
		if ((nwritten = write(fd, bufp, nleft)) <= 0) { // write error
			if (errno == EINTR)
				nwritten = 0; // prepare for next write
			else
				return -1; // other kind of error
		}
		// state update
		nleft -= nwritten;
		bufp += nwritten;
	}
	return n;
}
