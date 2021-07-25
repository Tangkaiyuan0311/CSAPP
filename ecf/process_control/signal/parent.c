#include "wrappers.h"
#define MAXBUF 100

void sigchild_handler1(int);
void sigchild_handler2(int);

int main(){
	int i, n;
	char buf[MAXBUF];
	

	// signal handler installation
	/*
	if (signal(SIGCHLD, sigchild_handler1) == SIG_ERR) // wrong handler
		unix_error("signal error");
	*/

	/*
	if (signal(SIGCHLD, sigchild_handler2) == SIG_ERR) //not portable installation
                unix_error("signal error");
	*/
	
	//portable installation
	Signal(SIGCHLD, sigchild_handler2); //wrapper has done the error handling


	//create 3 child processes
	for (i = 0; i < 3; i++){
		if (Fork() == 0){
			printf("hello from child: %d\n", (int)getpid());
			exit(0);
		}
	}

	// parent process do its own job
	if ((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0)
		unix_error("read error");
	
	printf("processing input: %s\n", buf);
	while (1)
		;
	exit(0);
}

