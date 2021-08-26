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

int Dup2(int oldfd, int newfd) {
	int retfd;
	if ((retfd= dup2(oldfd, newfd)) < 0)
		unix_error("dup2 error");
	return retfd;
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
// return: 0 on EOF, -1 on unrecoverable error, otherwise actually bytes read
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

ssize_t Rio_readn(int fd, void *ptr, size_t nbytes) 
{
    ssize_t n;
  
    if ((n = rio_readn(fd, ptr, nbytes)) < 0)
	unix_error("Rio_readn error");
    return n;
}

void Rio_writen(int fd, void *usrbuf, size_t n) 
{
    if (rio_writen(fd, usrbuf, n) != n)
	unix_error("Rio_writen error");
}

void Rio_readinitb(rio_t *rp, int fd)
{
    rio_readinitb(rp, fd);
} 

ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n) 
{
    ssize_t rc;

    if ((rc = rio_readnb(rp, usrbuf, n)) < 0)
	unix_error("Rio_readnb error");
    return rc;
}

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
{
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
	unix_error("Rio_readlineb error");
    return rc;
} 

int Socket(int domain, int type, int protocol) {
	int fd;
	if ((fd = socket(domain, type, protocol)) < 0)
		unix_error("socket error");
	return fd;
}

int Connect(int clientfd, const struct sockaddr* addr, socklen_t addrlen) {
	int ret;
	if ((ret = connect(clientfd, addr, addrlen)) < 0)
        unix_error("connect error");
    return ret;
}

int Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    int ret;
    if ((ret = bind(sockfd, addr, addrlen)) < 0)
        unix_error("bind error");
    return ret;
}

int Listen(int sockfd, int backlog) {
	int ret;
    if ((ret = listen(sockfd, backlog)) < 0)
        unix_error("listen error");
    return ret;
}
/*
The addrlen argument is a value-result argument: the caller must
initialize it to contain the size (in bytes) of the structure
pointed to by addr; on return it will contain the actual size of
the peer address.
*/
int Accept(int listenfd, struct sockaddr* addr, int* addrlen) {
	int connect_fd;
	if ((connect_fd = accept(listenfd, addr, addrlen)) < 0)
        unix_error("accept error");
    return connect_fd;
}

// establish a connection with server at (hostname, port)
// return an open socket descriptor
int open_clientfd(char* hostname, char* port) {
	int client_fd;
	struct addrinfo hints, *listp, *p;

	/* get a list of potential server socket address */
	// set up hints struct
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_socktype = SOCK_STREAM; // for connection
	hints.ai_flags = AI_NUMERICSERV; // numeric port arg instead of service name
	hints.ai_flags |= AI_ADDRCONFIG;
	// get address list
	getaddrinfo(hostname, port, &hints, &listp);

	/* walk the list and try to connect */
	for (p = listp; p; p = p->ai_next) {
		// try to create a socket
		if ((client_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
			continue;
		// try to connect
		if ((connect(client_fd, p->ai_addr, p->ai_addrlen)) != -1)
			break; // connect success
		Close(client_fd); // close the half open client socket
	}

	/* clean up */
	freeaddrinfo(listp);
	if (!p) // all connect attempts failed
		return -1;
	else
		return client_fd;
}

// return a open listening socket fd binded to (wildcard addr, port)
int open_listenfd(char* port) {
	struct addrinfo hints, *listp, *p;
	int listenfd, optval = 1;

	// get the list of available server address
	memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM; // for connection
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; // (widcard address)any IP address of this server is OK
	hints.ai_flags |= AI_NUMERICSERV; // numeric port arg instead of service name

    // get address list (each entry should be a wildcard address)
    getaddrinfo(NULL, port, &hints, &listp); // NULL hostname for passive socket address

	/* walk the list and try to connect */
    for (p = listp; p; p = p->ai_next) {
        // try to create a socket
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
            continue;

		// eliminate address already in use error from bind
		setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));

        // try to bind
        if ((bind(listenfd, p->ai_addr, p->ai_addrlen)) == 0)
            break; // bind success
        Close(listenfd); // close the half open server socket
    }

	/* clean up */
    freeaddrinfo(listp);
    if (!p) // all bind attempts failed
        return -1;

	/* make it a listening socket */
	if ((listen(listenfd, LISTENQ)) < 0) {
		Close(listenfd);
		return -1;
	}

	return listenfd; // ready to be used in accept
}
















