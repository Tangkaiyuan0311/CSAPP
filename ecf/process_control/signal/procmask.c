#include "wrappers.h"
#define MAXJOBS 100

void sigchild_handler(int);

struct {
	pid_t jobs[MAXJOBS];
	int jobnum;
} joblist;


void initjobs(){
	int i;
	for (i = 0; i < MAXJOBS; i++)
		joblist.jobs[i] = -1;
	joblist.jobnum = 0;
}

void addjob(pid_t pid){
	if (joblist.jobnum == MAXJOBS - 1){
		fprintf(stderr, "joblist full!\n");
		exit(0);
	}
	joblist.jobs[(joblist.jobnum)++] = pid;
}

void deletejob(pid_t pid){
        if (joblist.jobnum == 0){
                fprintf(stderr, "no job exit!\n");
                exit(0);
        }
        joblist.jobs[--(joblist.jobnum)] = -1;
}

int main(int argc, char** argv){
	pid_t pid;
	sigset_t mask_all, prev_all, mask_one;
	Sigemptyset(&mask_one);
	Sigaddset(&mask_one, SIGCHLD);
	Sigfillset(&mask_all);
	Signal(SIGCHLD, sigchild_handler);
	initjobs();

	while (1){
		Sigprocmask(SIG_BLOCK, &mask_one, &prev_all);
		if ((pid = Fork()) == 0){
			Sigprocmask(SIG_SETMASK, &prev_all, NULL);
			Execve("/bin/date", argv, NULL);
		}

		Sigprocmask(SIG_BLOCK, &mask_all, NULL);
                addjob(pid);
		//sleep(1);
                Sigprocmask(SIG_SETMASK, &prev_all, NULL);
	}
	exit(0);
}
