#include <stdio.h>
#include <stdlib.h>
#include "wrappers.h"


int main(){
	pid_t pid;

	printf("parent process:\n");
	print_pinfo();

	pid = Fork();

	//cannot assume which one returns first, child or parent

	if (pid == 0){ 
		printf("child process:\n");
		print_pinfo();
		printf("pause.....\n");
		Pause();
		printf("control should not reach here\n");
		exit(0);
	}

	//parent
	printf("parent process sleep for 5 secs...\n");
	sleep(5); //make sure child process is fully executed before sending out the signal
	printf("parent process sends out a kill signal:\n");
	Kill(pid, SIGKILL);
	int status;
	pid = waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
		printf("process%d terminated as the receipt of signal%d\n", pid, WTERMSIG(status));
	exit(0);
}


