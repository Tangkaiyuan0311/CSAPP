#include "wrappers.h"

// the pool of cnnected fd (connected client)
typedef struct {
	int maxfd;						// largest fd in the read_set
	fd_set read_set;				// input of select
	fd_set ready_set;				// modified by select
	int nready;						// number of ready descriptor returned from select
	int maxi;						// high water index into clientfd array
	int clientfd[FD_SETSIZE];		// active connect descriptors (one for each client)
	rio_t clientrio[FD_SETSIZE];	// active read buffers (one for each connected client)
} pool;

int byte_cnt = 0; // total bytes received by server

void init_pool(int listenfd, pool* p) {
	// initially, no connected fd
	p->maxi = -1;
	for (int i = 0; i < FD_SETSIZE; i++)
		p->clientfd[i] = -1;

	// listen fd is the only read_set member
	p->maxfd = listenfd;
	FD_ZERO(&p->read_set);
	FD_SET(listenfd, &p->read_set);
}

void add_client(int connfd, pool* p) {
	p->nready--; // listenfd in the ready_set that trigger the addition does not count towards ready clients
	int i;
	for (i = 0; i < FD_SETSIZE; i++) { // find an available empty slot
		if (p->clientfd[i] < 0) {
			// add the connected
			p->clientfd[i] = connfd;
			Rio_readinitb(&p->clientrio[i], connfd);

			// add connfd to read_set
			FD_SET(connfd, &p->read_set);

			// update maxfd and maxi
			if (connfd > p->maxfd)
				p->maxfd = connfd;
			if (i > p->maxi)
				p->maxi = i;
			break;
		}
	}

	if (i == FD_SETSIZE) { // could not find empty set
		fprintf(stderr, "no empty slot available\n");
		exit(0);
	}
}

// serve each ready client
void check_clients(pool* p) {
	int i, connfd, n;
	char buf[MAXLINE];
	rio_t rio;

	for (i = 0; (i <= p->maxi) && (p->nready > 0); i++) {
		connfd = p->clientfd[i];
		// if it's ready, serve it (EOF from the client connection can also trigger a I/O event(select return))
		if ((connfd > 0) && (FD_ISSET(connfd, &p->ready_set))) {
			rio = p->clientrio[i];
			p->nready--;
			if ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
				byte_cnt += n;
				printf("Server received %d (%d total) bytes on fd %d\n", n, byte_cnt, n);
				Rio_writen(connfd, buf, n);
			}
			else { // EOF on client: connfd
				Close(connfd);
				FD_CLR(connfd, &p->read_set);
				p->clientfd[i] = -1;
			}
		}
	}
}

int main(int argc, char **argv) {
	int listenfd, connfd;
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;
	static pool pool;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}

	listenfd = open_listenfd(argv[1]);
	init_pool(listenfd, &pool);

	while (1) {
		// wait for listenfd/connfds to become ready
		pool.ready_set = pool.read_set;
		pool.nready = select(pool.maxfd+1, &pool.ready_set, NULL, NULL, NULL);

		// an I/O event occurs

		// if new connection request arrive, add new client to the pool;
		if (FD_ISSET(listenfd, &pool.ready_set)) {
			clientlen = sizeof (struct sockaddr_storage);
			connfd = Accept(listenfd, (SA*)&clientaddr, &clientlen);
			add_client(connfd, &pool); // create a new state machine
		}

		// serve each ready clients (state transition for each state machine)
		check_clients(&pool);
	}

}












