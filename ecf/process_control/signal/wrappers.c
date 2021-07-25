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

#include <signal.h>

#include "wrappers.h"


void print_pinfo(){
        pid_t pid, ppid, pgid;
        pid = getpid();
        ppid = getppid();
        pgid = getpgrp();
        printf("pid=%d, ppid=%d, pgid=%d\n", pid, ppid, pgid);
}



void unix_error(char* msg){
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(0);
}


int Sigprocmask(int how, const sigset_t* set, sigset_t* oldset){
	int ret;
	if ((ret = sigprocmask(how, set, oldset)) == -1)
		unix_error("signal mask error");
	return ret;
}

int Sigemptyset(sigset_t* set){
	int ret;
        if ((ret = sigemptyset(set)) == -1)
                unix_error("set empty error");
        return ret;
}
int Sigaddset(sigset_t* set, int signum){
	int ret;
        if ((ret = sigaddset(set, signum)) == -1)
                unix_error("set add error");
        return ret;
}
int Sigfillset(sigset_t* set){
	int ret;
	if ((ret = sigfillset(set)) == -1)
                unix_error("set fill error");
        return ret;
}

int Sigismember(const sigset_t* set, int signum){
	int ret;
        if ((ret = sigismember(set, signum)) == -1)
                unix_error("check membership error");
        return ret;
}


int Pause(void){
	int ret;
	ret = pause();
	return ret;
}

void Execve(const char* filename, char* const argv[], char* const envp[]){
	if (execve(filename, argv, envp) == -1)
		unix_error("execve error");
}


pid_t Fork(void){
	pid_t pid;

	if((pid = fork()) < 0) // 3 types of return value
		unix_error("fork error");

	return pid;
}



int Kill(pid_t pid, int sig){
	int ret;
	if((ret = kill(pid, sig) < 0))
		unix_error("kill error");
	return ret;
}

char* Fgets(char *str, int n, FILE *stream){
  char* ret;
  if ((ret = fgets(str, n, stream)) == NULL)
    unix_error("fgets error");
  return ret;
}

//typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* Restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}



/* Private sio functions */

/* $begin sioprivate */
/* sio_reverse - Reverse a string (from K&R) */
static void sio_reverse(char s[])
{
    int c, i, j;

    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* sio_ltoa - Convert long to base b string (from K&R) */
static void sio_ltoa(long v, char s[], int b) 
{
    int c, i = 0;
    
    do {  
        s[i++] = ((c = (v % b)) < 10)  ?  c + '0' : c - 10 + 'a';
    } while ((v /= b) > 0);
    s[i] = '\0';
    sio_reverse(s);
}

/* sio_strlen - Return length of string (from K&R) */
static size_t sio_strlen(char s[])
{
    int i = 0;

    while (s[i] != '\0')
        ++i;
    return i;
}
/* $end sioprivate */

/* Public Sio functions */
/* $begin siopublic */

ssize_t sio_puts(char s[]) /* Put string */
{
    return write(STDOUT_FILENO, s, sio_strlen(s)); //line:csapp:siostrlen
}

ssize_t sio_putl(long v) /* Put long */
{
    char s[128];
    
    sio_ltoa(v, s, 10); /* Based on K&R itoa() */  //line:csapp:sioltoa
    return sio_puts(s);
}

void sio_error(char s[]) /* Put error message and exit */
{
    sio_puts(s);
    _exit(1);                                      //line:csapp:sioexit
}
/* $end siopublic */

/*******************************
 * Wrappers for the SIO routines
 ******************************/
ssize_t Sio_putl(long v)
{
    ssize_t n;
  
    if ((n = sio_putl(v)) < 0)
	sio_error("Sio_putl error");
    return n;
}

ssize_t Sio_puts(char s[])
{
    ssize_t n;
  
    if ((n = sio_puts(s)) < 0)
	sio_error("Sio_puts error");
    return n;
}

void Sio_error(char s[])
{
    sio_error(s);
}
  
  


