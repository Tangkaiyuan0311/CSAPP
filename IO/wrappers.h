#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include<errno.h>
#include<string.h>


int Open(char*, int, mode_t);
int Close(int);
