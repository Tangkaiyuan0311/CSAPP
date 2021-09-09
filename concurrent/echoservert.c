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
// serve the client on vargp
void* thread(void* vargp) {
	// read argument
	int connfd = *((int*)vargp);
	// detach itself
	Pthread_detach(pthread_self());
	free(vargp);
	echo(connfd); // descriptor table is shared among threads
	Close(connfd); // necessary
	return NULL;
}


int main(int argc, char** argv) {
	int listenfd, *connfdp;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;
	pthread_t tid;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}

	listenfd = open_listenfd(argv[1]);

	while (1) {
		clientlen = sizeof(struct sockaddr_storage);
		connfdp = malloc(sizeof(int));
		*connfdp = Accept(listenfd, (SA*)&clientaddr, &clientlen);
		Pthread_create(&tid, NULL, thread, connfdp);
	}
}
