#define MAXARGS 128
#define MAXLINE 1024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wrappers.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


extern char **environ;

void eval(char* cmdline);
int parseline(char* buf, char** argv);
int builtin_command(char** argv);

//interesting when > hello &, a blank line, a missing > ???

//read and evaluate command line
int main(){
  char cmdline[MAXARGS];
  
  while(1){
    //read in command
    printf("> ");
    //fprintf(stderr, "> ");
    //fflush(stdout);
    Fgets(cmdline, MAXLINE, stdin);
    if (feof(stdin)) //EOF from stdin
      exit(0);
    //evaluate
    eval(cmdline);
    //fflush(stdout);
  }
}

int parseline(char* buf, char** argv){ //buf itself will be modified

  char* delim;
  int bg;

  buf[strlen(buf)-1] = ' '; //remove trailing '\n'
  while (buf && (*buf == ' ')) //remove leading ' 's
    buf++;

  //build up argv array
  int argc = 0;
  while (delim = strchr(buf, ' ')){ //find the first occurrence of ' ', exit when meeting '\0'
    argv[argc++] = buf;
    *delim = '\0';
    buf = delim + 1;
    while (buf && (*buf == ' ')) //remove ' 's
      buf++;
  }
  argv[argc] = NULL;

  if (argc == 0) //blank line
    return 1;

  //check for back ground, fore ground
  if (bg = (*argv[argc-1] == '&'))
    argv[--argc] = NULL;

  return bg;
}


void eval(char* cmdline){

  int bg;
  char buf[MAXLINE];
  char* argv[MAXARGS];
  pid_t pid;
  
  strcpy(buf, cmdline);
  bg = parseline(buf, argv);
  if (argv[0] == NULL) //ignore empty line
    return;
  
  if (!builtin_command(argv)){
    pid = Fork();
    if (pid == 0){ //child process, run the provided program
      if (execve(argv[0], argv, environ) < 0){ //error handling
	printf("%s: Command not found\n", argv[0]);
	exit(0);
      }
    }
  }

  if (!bg){ //fore ground, wait the child process
    int status;
    if (waitpid(pid, &status, 0) < 0)
      unix_error("waitpid error:");
    if (WIFEXITED(status))
      printf("child: %d teminated normally with exit status=%d\n", pid, WEXITSTATUS(status));
  }
  else
    printf("%d : %s\n", pid, cmdline);//back ground

}

int builtin_command(char** argv){
  if (strcmp(argv[0], "quit") == 0)
    exit(0);
  if (strcmp(argv[0], "&") == 0)
    return 1; //ignore
  return 0; // not built in command, run the program
}
  

  
      
      
  






  
    
    
