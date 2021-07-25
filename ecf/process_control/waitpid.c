#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
// pid_t waitpid(pid_t pid, int* statusp, int options);
// return child pid on success
// default: options = 0
// pid > 0, pid = -1
// options: WNOHANG, WUNTRACED, WCONTINUED, WNOHANG | WUNTRACED, ....
// status interpretation, WIFEXITED(status).....
// error conditions: return -1, errno = ECHILD/EINTR
//
// pid_t wait(int* statusp); -> waitpid(-1, statusp, 0);

#define N 5

pid_t Fork(void);
void unix_error(char*);

#ifndef ORDER

int main(){

	int i;
	pid_t pid;
	int status;
	
	//create process
	for(i = 0; i < N; i++){
		pid = Fork();
		if(pid == 0){ //child
			exit(i);//teminate immediately and normally
		}
	}

	printf("sleep for 5 secs\n");

	int remain = sleep(5);

	printf("%d secs remains now\n", remain);
		
	printf("reap %d children in undeterministic order\n", N);

	while ((pid = waitpid(-1, &status, 0)) > 0){
		if (WIFEXITED(status)) //check for exit status
			printf("child %d exit normally with exit status: %d\n", pid, WEXITSTATUS(status));
		
		else
			printf("child %d exit abnormally\n", pid);
	}

	if (errno != ECHILD)
		unix_error("waitpid error");
	exit(0);
}

#endif

#ifdef ORDER

int main(){

        int i;
        pid_t pid;
	pid_t pids[N];
        int status;

	
	//create process
        for(i = 0; i < N; i++){
                pid = Fork();
                if(pid == 0){ //child
                        exit(i);//teminate immediately and normally
                }
		pids[i] = pid; //record child pid
        }
	//reap children process in specific order(reverse order)
	
	printf("reap %d children in reverse order:\n", N);
/*
	for (i = N-1; i >= 0; i--){

        	pid = waitpid(pids[i], &status, 0);
                if (WIFEXITED(status)) //check for exit status
                        printf("child %d exit normally with exit status: %d\n", pid, WEXITSTATUS(status));

                else
                        printf("child %d exit abnormally\n", pid);
       }
 */
	
	i = N-1;
	while ((pid = waitpid(pids[i--], &status, 0)) > 0){
		if (WIFEXITED(status)) //check for exit status
                        printf("child %d exit normally with exit status: %d\n", pid, WEXITSTATUS(status));

                else
                        printf("child %d exit abnormally\n", pid);
	}



        if (errno != ECHILD)
                unix_error("waitpid error");
        exit(0);
}

#endif



