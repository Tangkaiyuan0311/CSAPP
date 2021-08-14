#include "wrappers.h"

int main(int argc, char** argv) {
	uint32_t addr; // host byte order
	struct in_addr inaddr; // network byte order
	char buf[1024]; // dotted decimal string

	if (argc != 2) {
		fprintf(stderr, "usage: %s <hex number>\n", argv[0]);
		exit(0);
	}

	printf("convert a IPV4 address(hex value) in host byte order to network byte order and show its representation\nfrom low address to high address(high order to low order):\n");

	sscanf(argv[1], "%x", &addr); // store the argument in host byte order
	inaddr.s_addr = htonl(addr);
	inet_ntop(AF_INET, &inaddr, buf, 1024);

	printf("%s\n", buf);

	exit(0);


}
