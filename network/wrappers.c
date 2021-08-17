#include "wrappers.h"


void unix_error(char* msg){
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
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

int Accept(int listenfd, struct sockaddr* addr, int* addrlen) {
	int connect_fd;
	if ((connect_fd = accept(listenfd, addr, addrlen)) < 0)
        unix_error("accept error");
    return connect_fd;
}


















