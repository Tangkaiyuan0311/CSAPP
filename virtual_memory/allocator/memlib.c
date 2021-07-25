#define MAX_HEAP (1<<14)
#include "wrappers.h"

// private global variable
static char* mem_heap; //first byte of heap
static char* mem_brk; //one past the last byte of heap(invalid address)
static char* mem_max_addr; //one past the max legal heap address(swap space)

/* mem_init - initialize the memory system model */
void mem_init(void){
	mem_heap = (char*)Malloc(MAX_HEAP);
	mem_brk = mem_heap;
	mem_max_addr = mem_heap + MAX_HEAP;
}

void mem_close(void){
	Free(mem_heap);
}

/*  mem_sbrk - extend the heap area by incr bytes, return start address of new area, -1 on error */
void* mem_sbrk(int incr){
	char* old_brk = mem_brk;

	if (incr < 0 || ((mem_brk + incr) > mem_max_addr)){
		errno = ENOMEM;
		fprintf(stderr, "mem_sbrk fail: ran out of memory...\n");
		return (void*)-1;
	}

	mem_brk += incr; //virtual pages allocation
	return (void*)old_brk;
}
