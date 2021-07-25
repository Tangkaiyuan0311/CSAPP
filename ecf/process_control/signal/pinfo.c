#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include "wrappers.h"

void print_pinfo(){
	pid_t pid, ppid, pgid;
	pid = getpid();
	ppid = getppid();
	pgid = getpgrp();
	printf("currrent process info:\npid=%d, ppid=%d, pgid=%d\n", pid, ppid, pgid);
}

