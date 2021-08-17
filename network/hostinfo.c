#include "wrappers.h"

int main(int argc, char** argv) {
	struct addrinfo *listp, *p, hints;
	char buf[1024];
	int rc, flags;
	/*
	printf("uninitialized pointer p = %p, listp = %p, hints = %p\n", p, listp, &hints);
	*/
	if (argc != 2) {
		fprintf(stderr, "usage: %s <domain name>\n", argv[0]);
		exit(0);
	}
	// argv[1] should be the target hostname


	// set up hint struct
	memset(&hints, 0, sizeof(struct addrinfo)); // cannot supply uninitialized struct pointer(segmentation fault)
	hints.ai_family = AF_INET; // return only IPv4 address
	hints.ai_socktype = SOCK_STREAM; // return only socket for connection
	hints.ai_flags = AI_CANONNAME; // include canonical name of the target host

	// get addrinfo list
	if((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
		exit(1);
	}
	/*
	printf("now, listp = %p\n",listp);
	*/
	// walk through the returned list
	flags = NI_NUMERICHOST; // display address string instead of host name
	printf("cannonical name: %s\n", listp->ai_canonname);
	for (p = listp; p; p = p->ai_next) {
		getnameinfo(p->ai_addr, p->ai_addrlen, buf, 1024, NULL, 0, flags);
		printf("%s\n", buf);
	}

	// clean up
	freeaddrinfo(listp);
	exit(0);
}
