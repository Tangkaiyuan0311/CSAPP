#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

void unix_error(char*);
FILE* Fopen(const char *, const char *);
void* Mmap(void*, size_t, int, int, int, off_t);
ssize_t Write(int, const void *, size_t);
