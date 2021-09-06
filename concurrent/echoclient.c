#include "wrappers.h"

int main(int argc, char** argv) {
	int clientfd;
	char* host, *port, buf[MAXLINE];
	rio_t rio;

	if (argc != 3) {
		fprintf(stderr, "usage: %s: <host> <port>\n", argv[0]);
		exit(0);
	}

	host = argv[1];
	port = argv[2];

	// establish the connection with server and initialize read buffer
	if ((clientfd = open_clientfd(host, port)) == -1) {
		fprintf(stderr, "connect failed\n");
		exit(0);
	}
	printf("<connection established>\n");
	Rio_readinitb(&rio, clientfd);

	printf("<wait for terminal input>:\n");
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		// send content to server
		printf("<write to server>\n");
		Rio_writen(clientfd, buf, strlen(buf));
		// read response from server
		printf("<read response from server>\n");
		Rio_readlineb(&rio, buf, MAXLINE);
		printf("<write response to stdout>\n");
		fputs(buf, stdout);
	}
	Close(clientfd);
	exit(0);
}
