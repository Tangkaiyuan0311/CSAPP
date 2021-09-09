#include "wrappers.h"

#define N 5

char** ptr;

void* thread(void* vargp) {
	int myid = (int)vargp;
	static int cnt = 0;
	printf("[%d]: %s (cnt = %d)\n", myid, ptr[myid], ++cnt);
	return NULL;
}

int main() {
	int i;
	pthread_t tid;
	char* msgs[N] = {
		"hello from 0",
		"hello from 1",
		"hello from 2",
		"hello from 3",
		"hello from 4"
	};
	ptr = msgs;
	for (i = 0; i < N; i++)
		Pthread_create(&tid, NULL, thread, (void*)i);
	pthread_exit(NULL);
}
