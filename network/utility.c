struct in_addr {
    uint32_t s_addr; // big-endian byte order
};

// generic socket address to simplify socket interface design (expected by connect, bind, accept), in lack of void*
// users cast their protocol specific struct pointer to this generic strct pointer as argument
// 2B + 14B
struct sockaddr {
	uint16_t	sa_family;		// protocol family
	char		sa_data[14];	// address data
}

// IP socket address (protocol specific address)
// 2B + 6B + 8B
struct sockaddr_in {
	uint16_t		sin_family;		// protocol family, always AF_INET
	uint16_t		sin_port;		//port number in network byte order
	struct in_addr	sin_addr;		//IP address in network byte order
	unsigned char	sin_zero[8];	//pad to sizeof(struct sockaddr)
}

struct addrinfo {
	int					ai_flags;		// hints argument flag
	int					ai_family;		// first argument to socket
	int					ai_socktype;	// second argument to socket
	int					ai_protocol;	// third argument to socket
	char*				ai_canonname;	// canonical hostname
	size_t				ai_addrlen;		// size of ai_addr struct
	struct sockaddr*	ai_addr;		// ptr to the socket address struct
	struct addrinfo*	ai_next;		// next item in the linked list
}








