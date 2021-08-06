#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include<errno.h>
#include<string.h>
#define RIO_BUFSIZE 8192

// struct for the internal buffer
typedef struct {
    int rio_fd; // associated open file descrioptor
    int rio_cnt; // unread bytes count in current buffer
    char* rio_bufptr; // next unread byte in the buffer
    char rio_buf[RIO_BUFSIZE]; // internal buffer
} rio_t;

int Open(char*, int, mode_t);
int Close(int);

ssize_t rio_readn(int fd, void* usrbuf, size_t n);
ssize_t rio_writen(int fd, void* usrbuf, size_t n);

void rio_readinitb(rio_t* rp, int fd);

ssize_t rio_readlineb(rio_t* rp, void* usrbuf, size_t maxlen);
ssize_t rio_readnb(rio_t* rp, void* usrbuf, size_t n);
