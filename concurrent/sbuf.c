#include "wrappers.h"
#include "sbuf.h"

// empty, bounded, shared FIFO buffer
void sbuf_init(sbuf_t* sp, int n) {
	sp->buf = (int*)calloc(n, sizeof(int));
	sp->n = n;
	sp->front = sp->rear = 0;
	sem_init(&sp->mutex, 0, 1);
	sem_init(&sp->slots, 0, n);
	sem_init(&sp->items, 0, 0);
}

void sbuf_deinit(sbuf_t* sp) {
	free(sp->buf);
}

// insertion
void sbuf_insert(sbuf_t* sp, int item) {
	P(&sp->slots); // suspend if full buffer

	P(&sp->mutex);
	sp->buf[(++sp->rear)%(sp->n)] = item; // no need to worry abput overflow(), semaphore sp->slots do the job
	V(&sp->mutex);

	V(&sp->items); // resume suspended deletion
}

// remove and return
int sbuf_remove(sbuf_t* sp) {
	int item;
	P(&sp->items); // suspend if empty buffer

	P(&sp->mutex);
    item = sp->buf[(++sp->front)%(sp->n)]; // no need to worry abput underflow(), semaphore sp->items do the job
    V(&sp->mutex);

	V(&sp->slots); // resume suspended insert
	return item;
}
