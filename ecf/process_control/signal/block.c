#include "wrappers.h"

void sigint_handler(int sig){
	Sio_puts("caught SIGINT!\n");
	return;
}

int main(){

	if (signal(SIGINT, sigint_handler) == SIG_ERR)
                unix_error("signal error");
        printf("successfully install SIGINT handler\n");

	sigset_t mask, prev_mask;

	Sigemptyset(&mask);
	Sigaddset(&mask, SIGINT);

	Sigprocmask(SIG_BLOCK, &mask, &prev_mask); //block SIGINT

	if (Sigismember(&prev_mask, SIGINT) == 1)
		printf("SIGINT has already been blocked\n");
	else
	       	printf("block SIGINT\n");
	printf("uniterrupted 10 secs' sleep...\n");
	int remain = sleep(10);
	printf("sleep for %d of 10 secs\n", 10-remain);

	Sigprocmask(SIG_SETMASK, &prev_mask, NULL); //unblocked SIGINT
	printf("restore default block set\n");

	printf("10 secs' sleep, can be interrupted...\n");
        remain = sleep(10);
        printf("sleep for %d of 10 secs\n", 10-remain);

	return 0;
}

