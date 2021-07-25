#include "wrappers.h"

volatile sig_atomic_t pid;

void sigchld_handler(int sig){
	int olderrno = errno;
	
	pid = waitpid(-1, NULL, 0); //atomic operation

	errno = olderrno;
}

void sigint_handler(int sig){
}

int main(int argc, char** argv){
	sigset_t mask, mask_all, prev;
	Signal(SIGCHLD, sigchld_handler);
	Signal(SIGINT, sigint_handler);
	Sigemptyset(&mask);
	Sigfillset(&mask_all);
	Sigaddset(&mask, SIGCHLD); //block SIGCHLD

	while (1){
		Sigprocmask(SIG_BLOCK, &mask, &prev); // block SIGCHLD
		if (Fork() == 0)
			exit(0);

		pid = 0;
		while (!pid){
			sigsuspend(&prev); // loop: unblock -> block
		}
		Sigprocmask(SIG_BLOCK, &mask_all, NULL);//protect access to pid
		printf("reap a foreground child process: %d\n", pid);
		Sigprocmask(SIG_SETMASK, &prev, NULL); // unblock SIGCHLD
	}

	exit(0);
}

		

