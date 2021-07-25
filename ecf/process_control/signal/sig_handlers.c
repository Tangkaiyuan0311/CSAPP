#include "wrappers.h"

void deletejob(pid_t);

//safe SIGINT handler

void sigint_handler(int sig){
	Sio_puts("Caught SIGINT!\n");
	_exit(0);
}


void sigchild_handler1(int sig){  //buggy
	int olderron = errno;
	
	pid_t pid;

	if ((pid = waitpid(-1, NULL, 0)) < 0){
		Sio_error("wait pid error");
	}

	Sio_puts("reap child: ");
	Sio_putl(pid);
	Sio_puts("\n");
	
	sleep(1);
		
	errno = olderron;
}

void sigchild_handler2(int sig){ 
        int olderron = errno;
        
        pid_t pid;

        while ((pid = waitpid(-1, NULL, 0)) > 0){
		Sio_puts("reap child: ");
       		Sio_putl(pid);
        	Sio_puts("\n");
	}

	if (errno != ECHILD)
		Sio_error("waitpid error");

        
        sleep(1);

        errno = olderron;
}

void sigchild_handler(int sig){
	int olderrno = errno;

	sigset_t mask_all, prev_all;
	pid_t pid;
	Sigfillset(&mask_all);

	while ((pid = waitpid(-1, NULL, 0)) > 0){
		Sio_puts("reap child: ");Sio_putl(pid);Sio_puts("\n");
		Sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
		deletejob(pid);
		Sigprocmask(SIG_SETMASK, &prev_all, NULL);
	}

	if (errno != ECHILD)
		Sio_error("waidpid error");

	errno = olderrno;
}




