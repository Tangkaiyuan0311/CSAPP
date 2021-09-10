#include "wrappers.h"
#define MAXTHREADS 32

void* sum_mutex(void* vargp);

// global shared variable
long gsum = 0;
long ele_per_thread;
sem_t mutex;

int main(int argc, char** argv) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s <nthreads> <log_nelems>\n", argv[0]);
		exit(0);
	}

	long i, nelems, log_nelems, nthreads, myid[MAXTHREADS];
	pthread_t tids[MAXTHREADS];

	nthreads = atoi(argv[1]);
	log_nelems = atoi(argv[2]);
	nelems = (1L << log_nelems); // 2^log_nelems
	ele_per_thread = nelems / nthreads;
	sem_init(&mutex, 0, 1);

	// create threads
	for (i = 0; i < nthreads; i++) {
		myid[i] = i;
		Pthread_create(&tids[i], NULL, sum_mutex, &myid[i]);
	}

	for (i = 0; i < nthreads; i++)
		Pthread_join(tids[i], NULL);

	if (gsum != (nelems * (nelems - 1))/2)
		fprintf(stderr, "Error: result = %ld\n", gsum);
	exit(0);
}

void* sum_mutex(void* vargp) {
	long myid = *((long*)vargp);
	long start = myid * ele_per_thread;
	long end = start + ele_per_thread;
	long i;

	for (i = start; i < end; i++) {
		P(&mutex);
		gsum += i;
		V(&mutex);
	}
	return NULL;
}
