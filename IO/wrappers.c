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


int Stat(const char* filename, struct stat* buf) {
	int ret;
	if((ret = stat(filename, buf)) < 0)
		unix_error("stat error");
	return ret;
}

int Fstat(int fd, struct stat* buf) {
	int ret;
    if((ret = fstat(fd, buf)) < 0)
        unix_error("fstat error");
    return ret;
}

DIR* Opendir(const char* name) {
	DIR* dirp = opendir(name);
	if (dirp == NULL)
		unix_error("opendir error");
	else
		return dirp;
}

int Closedir(DIR* dirp) {
	int ret;
	if ((ret = closedir(dirp)) < 0)
		unix_error("closedir error");
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

// initialize the internal buffer with given descriptor
// assocaite fd with read buffer at rp
void rio_readinitb(rio_t* rp, int fd) {
	rp->rio_fd = fd;
	rp->rio_cnt = 0;
	rp->rio_bufptr = rp->rio_buf;
}

// buffered vresion of read with exact semantics
// return: 0 on EOF, -1 on unrecoverable error, aotherwise actually bytes read
static ssize_t rio_read(rio_t* rp, char* usrbuf, size_t n) {
	int cnt; // number of bytes actually read

	while (rp->rio_cnt <= 0) { // buffer is empty or previous refilling request trigger a recoverable error
		// try to refill the internal buffer
		rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));

		if (rp->rio_cnt < 0) { //error
			if (errno != EINTR) // unrecoverable error(not sig handler return)
				return -1;
		}
		else if (rp->rio_cnt == 0) // EOF
			return 0;
		else // read success
			rp->rio_bufptr = rp->rio_buf; // reset the buffer pointer to the beginning of buffer, ready
	}

	// internal buffer has been set up

	// copy min(n, rp->rio_cnt) bytes to usrbuf
	cnt = n;
	if (rp->rio_cnt < n)
		cnt = rp->rio_cnt;
	// copy
	memcpy(usrbuf, rp->rio_bufptr, cnt);

	// buffer state update
	rp->rio_bufptr += cnt;
	rp->rio_cnt -= cnt;
	
	return cnt;
}

// read a line of at most maxlen-1 bytes from rp, terminates with '\0'
// return: actually bytes read(excluding '\0'), -1 on error
ssize_t rio_readlineb(rio_t* rp, void* usrbuf, size_t maxlen) {
	int n; // character counter
	int rc; //
	char c; // read character holder
	char* bufp = usrbuf;

	for (n = 1; n < maxlen; n++) { // read at most maxlen-1 bytes
		// n-1 charracters read now
		if ((rc = rio_read(rp, &c, 1)) == 1) {
			*(bufp++) = c; // store read characters
			if (c == '\n') {
				n++; 
				break;
			}
		}
		else if (rc == 0) { // most recent rio_read trigger an EOF
			if (n == 1) // no data read before EOF occurred
				return 0;
			else // some data is read before EOF occurred
				break;
		}
		else // rc == -1
			return -1; // error
	}

	*bufp = 0; // terminating NULL character
	return n-1;
}

// buffered version of readnb
ssize_t rio_readnb(rio_t* rp, void* usrbuf, size_t n) {
	size_t nleft = n; //initialization
    ssize_t nread; // number of bytes read on each call to rio_read(buffered), 0 on EOF, -1 on unrecoverable error
    char* bufp = usrbuf;

    while (nleft > 0) { // request is not satisfied yet
        if ((nread = rio_read(rp, bufp, nleft)) < 0) { // rio_read error
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





