#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
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
#include <signal.h>
#include <sys/select.h>
/*
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
FD_ZERO(fd_set *fdset); // clear all bits
FD_CLR(int fd, fd_set *fdset); // clear fd bit
FD_SET(int fd, fd_set *fdset); // set fd bit
FD_ISSET(int fd, fd_set *fdset); // if fd bit in the fdset on
 */
#include <pthread.h>
#include <semaphore.h>
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
typedef void handler_t(int);

void unix_error(char*);

int Open(char*, int, mode_t);
int Close(int);

int Stat(const char* filename, struct stat* buf);
int Fstat(int fd, struct stat* buf);

DIR* Opendir(const char* name);
int Closedir(DIR* dirp);

int Dup2(int oldfd, int newfd);
pid_t Fork(void);
void Execve(const char *filename, char *const argv[], char *const envp[]);
pid_t Wait(int *status);

handler_t* Signal(int signum, handler_t* handler);
int Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
int Pthread_detach(pthread_t tid);
int Pthread_join(pthread_t thread, void **retval);
void P(sem_t *s);
void V(sem_t *s);
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