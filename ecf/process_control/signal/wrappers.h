#include <sys/types.h>

#include <unistd.h>

#include <errno.h>

#include <string.h>

#include <stdio.h>

#include <stdlib.h>

#include <signal.h>

#include <sys/wait.h>

#include <setjmp.h>



typedef void handler_t(int); 

/* Sio (Signal-safe I/O) routines */
ssize_t sio_puts(char s[]);
ssize_t sio_putl(long v);
void sio_error(char s[]);

/* Sio wrappers */
ssize_t Sio_puts(char s[]);
ssize_t Sio_putl(long v);
void Sio_error(char s[]);

handler_t* Signal(int, handler_t*);
void Execve(const char*, char* const [], char* const []);
int Kill(pid_t, int);
pid_t Fork(void);
int Pause(void);
char* Fgets(char*, int, FILE*);
int Sigprocmask(int, const sigset_t*, sigset_t*);
int Sigemptyset(sigset_t*);
int Sigaddset(sigset_t*, int);
int Sigfillset(sigset_t*);
int Sigismember(const sigset_t*, int);
void unix_error(char*);
void print_pinfo();
