#include <stdio.h>
#include <sys/types.h>
//pid_t

#include <unistd.h>
//pid_t getpid(void);
//pid_t getppid(void);


int main(){
	pid_t pid, ppid;
	pid = getpid();
	ppid = getppid();
	printf("pid = %d, ppid = %d\n", pid, ppid); //running from the shell, ppid is the shell pid?
	return 0;
}
