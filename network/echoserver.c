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

int main(int argc, char** argv) {
	int listenfd, connfd;
	socklen_t clientlen; // size of client sockaddr struct
	struct sockaddr_storage clientaddr; // enough space for any address type
	char client_hostname[MAXLINE], client_port[MAXLINE];

	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}

	listenfd = open_listenfd(argv[1]);
	/*
	if ((listenfd = socket( AF_INET, SOCK_STREAM, 0 )) == -1) {
		fprintf(stderr, "socket error\n");
		exit(0);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	addr.sin_port = htons( (uint32_t)atoi(argv[1]) );
	if (bind( listenfd, (SA*)&addr, sizeof(SA) ) == -1) {
        fprintf(stderr, "bind error\n");
        exit(0);
    }
	if ((listen(listenfd, LISTENQ)) < 0) {
		fprintf(stderr, "listen error\n");
        Close(listenfd);
        exit(0);
    }
	*/
	while (1) {
		clientlen = sizeof(struct sockaddr_storage);
		connfd = Accept(listenfd, (SA*)&clientaddr, &clientlen);
		getnameinfo((SA*)&clientaddr, clientlen,
				client_hostname, MAXLINE, 
				client_port, MAXLINE, 0);
		printf("connected to (%s, %s)\n", client_hostname, client_port);
		echo(connfd);
		Close(connfd);
	}
	exit(0);
}
