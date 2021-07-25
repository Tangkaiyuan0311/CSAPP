#include "wrappers.h"
#include <time.h>

void sigalrm_handler(int sig){ //buggy one
	time_t t = time(NULL);
	printf("signal %d is caught at time: %ld\n", sig, t);
	return; //do not terminate
}

void sigint_handler(int sig){ //buggy one
	printf("SIGINT is caught\n");
	return; //do not terminate
}


int main(int argc, char** argv){
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		unix_error("signal error");
	printf("successfully install SIGINT handler\n");
	if (signal(SIGALRM, sigalrm_handler) == SIG_ERR)
		unix_error("signal error");
	printf("successfully install SIGALRM handler\n");

	time_t t = time(NULL);
	printf("send SIGALRM 5 secs after %ld\n", t);
	int ret;
	if (ret = alarm(5) != 0){
		printf("previous alarm unfinished\n");
		exit(0);
	}
	printf("wait....\n");
	Pause();
	printf("wait for SIGINT now...\n");
	Pause();
	int time = atoi(argv[1]);
	printf("sleep for %d secs...\n", time);
	
	int remain;
	remain = sleep(time);
	printf("sleep for %d of %d secs\n", time-remain, time);

	return 0;
}



