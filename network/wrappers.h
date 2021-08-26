#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
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
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#define RIO_BUFSIZE 8192
#define MAXLINE 1024
#define LISTENQ 1024

// struct for the internal buffer
typedef struct {
    int rio_fd; // associated open file descrioptor
    int rio_cnt; // unread bytes count in current buffer
    char* rio_bufptr; // next unread byte in the buffer
    char rio_buf[RIO_BUFSIZE]; // internal buffer
} rio_t;

typedef struct sockaddr SA;

void unix_error(char*);

int Open(char*, int, mode_t);
int Close(int);

int Stat(const char* filename, struct stat* buf);
int Fstat(int fd, struct stat* buf);

DIR* Opendir(const char* name);
int Closedir(DIR* dirp);

int Dup2(int oldfd, int newfd);

ssize_t rio_readn(int fd, void* usrbuf, size_t n);
ssize_t rio_writen(int fd, void* usrbuf, size_t n);

void rio_readinitb(rio_t* rp, int fd);

ssize_t rio_readlineb(rio_t* rp, void* usrbuf, size_t maxlen);
ssize_t rio_readnb(rio_t* rp, void* usrbuf, size_t n);

/* Wrappers for Rio package */
ssize_t Rio_readn(int fd, void *usrbuf, size_t n);
void Rio_writen(int fd, void *usrbuf, size_t n);
void Rio_readinitb(rio_t *rp, int fd); 
ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

void unix_error(char*);
int Socket(int domain, int type, int protocol);
int Connect(int clientfd, const struct sockaddr* addr, socklen_t addrlen);
int Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int listenfd, struct sockaddr* addr, int* addrlen);
int open_clientfd(char* hostname, char* port);
int open_listenfd(char* port);
