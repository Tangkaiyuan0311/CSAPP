#include<sys/types.h>
//define pid_t as int on linux

#include<unistd.h>
//pid_t getpid(void);
//pid_t fork(void);


#include<errno.h>
//define errno

#include<string.h>
//declare strerror

#include<stdio.h>
//stderr

#include<stdlib.h>
//fprintf, exit, fgets



void unix_error(char* msg){
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
}

pid_t Fork(void){
	pid_t pid;

	if((pid = fork()) < 0) // 3 types of return value
		unix_error("fork error");

	return pid;
}


char* Fgets(char *str, int n, FILE *stream){
  char* ret;
  if ((ret = fgets(str, n, stream)) == NULL)
    unix_error("fgets error");
  return ret;
}
  
  


