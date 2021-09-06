#include "wrappers.h"

void echo(int connfd) {
	size_t n;
	char buf[MAXLINE];
	rio_t rio;

	Rio_readinitb(&rio, connfd);
	printf("wait for client input\n");
	while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
		printf("server received %d bytes\n", (int)n);
		Rio_writen(connfd, buf, n);
	}
}

void sigchld_handler(int sig) {
	while (waitpid(-1, 0, WNOHANG) > 0) // return 0 if no child in the waitset waiting to be reaped
		;
	return;
}

int main(int argc, char **argv) {
	int listenfd, connfd;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}

	Signal(SIGCHLD, sigchld_handler);
	/*
	if ((listenfd = open_listenfd(argv[1])) < 0) {
		fprintf(stderr, "open_listenfd error\n");
		exit(0);
	}
	*/

	listenfd = open_listenfd(argv[1]);
	
	while (1) {
		clientlen = sizeof(struct sockaddr_storage);
		connfd = Accept(listenfd, (SA*)&clientaddr, &clientlen);
		// fork a child to serve client from connfd
		if (Fork() == 0) {
			Close(listenfd); // optional
			echo(connfd);
			Close(connfd); // close the server side of socket connection
			exit(0);
		}

		Close(connfd); // necessary to avoid memory leaks
	}

}





















