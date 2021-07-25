#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>


pid_t Fork(void);

int main(){
	pid_t pid;
	int x = 1;

	pid = Fork();
	
	if(pid == 0){//child process
		printf("child: x = %d\n", ++x);
		fflush(stdout);
	       	exit(0);
	}
	//parent process
	printf("parent: x = %d\n", --x);
	fflush(stdout);
	exit(0);
}


