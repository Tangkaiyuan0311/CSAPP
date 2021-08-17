#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
/* 
#include <netinet/in.h>
struct in_addr {
	uint32_t s_addr; // big-endian byte order
};
*/

#include <sys/types.h>
#include <sys/socket.h>
/*
int socket(int domain, int type, int protocol); 
int connect(int clientfd, const struct sockaddr* addr, socklen_t addrlen);
int listen(int sockfd, int backlog);
int accept(int listenfd, struct sockaddr* addr, int* addrlen);
*/
#include <netdb.h>

void unix_error(char*);
int Socket(int domain, int type, int protocol);
int Connect(int clientfd, const struct sockaddr* addr, socklen_t addrlen);
int Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int listenfd, struct sockaddr* addr, int* addrlen);
