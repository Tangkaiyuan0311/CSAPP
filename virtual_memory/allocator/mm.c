#include "memlib.h"
#include "wrappers.h"

static char* heap_listp;

/* basic constants and macros */
#define WSIZE	4 //header/footer size
#define DSIZE	8 //alignment
#define CHUNKSIZE  (1<<12) //default heap extention

#define MAX(x, y) (((x) > (y))? (x) : (y))

// pack size bits and allocated bit into a word
#define PACK(size, alloc) ((size) | (alloc))

// read and write word at addr p
#define GET(p) (*((unsigned int*)(p)))
#define PUT(p, val) (*((unsigned int*)(p)) = (val))

// read size and alloc bits from header or footer at addr p
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

// operate block pointer bp
#define HDRP(bp) ((char*)(bp) - WSIZE)
#define FTRP(bp) ((char*)bp + GET_SIZE(HDRP(bp)) - DSIZE)

#define NEXT_BLKP(bp) ((char*)(bp) + GET_SIZE(HDRP(bp)))
#define PREV_BLKP(bp) ((char*)(bp) - GET_SIZE((char*)(bp) - DSIZE))

static void* extend_heap(size_t);
// initialize heap with suitable format and an initial free block, return -1 on error
int mm_init(void){
	//format initialization
	if ((heap_listp = (char*)mem_sbrk(4*WSIZE)) == (void*)-1)
		return -1;
	PUT(heap_listp, 0); // alignment padding
	PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1)); // prologue header
	PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1)); // prologue footer
	PUT(heap_listp + (3*WSIZE), PACK(0, 1)); // epilogue header
	heap_listp += (2*WSIZE); // minor optimization???

	// extend the empty with a free block of CHUNKSIZE bytes
	if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
		return -1;
	return 0;
}

static void* coalesce(void*);

// extend the heap with aligned number of words, initialize with free pointer, 
// return new free block pointer, NULL on error
static void* extend_heap(size_t words){
	char* bp;
	size_t size;

	// allocate even number of words(rounded up) to maintain alignment
	size = (words%2) ? (words+1)*WSIZE : words*WSIZE;
	if ((bp = mem_sbrk(size)) == (void*)-1)
		return NULL;

	// initialize the extended heap area as a free block
	PUT(HDRP(bp), PACK(size, 0)); //free block header, overwrite previous epilogue(header)
	PUT(FTRP(bp), PACK(size, 0)); //free block footer
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); //new epilogue header

	// possible coalesce
	return coalesce(bp);
}

void mm_free(void* bp){
	size_t size = GET_SIZE(HDRP(bp));

	//free: update header and footer
	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));

	coalesce(bp);
}


//boundary_tag coalescing current free block with its neighbours, return new free block pointer
static void* coalesce(void* bp){
	//get neighbouring allocate bit and current block size
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));

	//considering 4 cases
	if (prev_alloc && next_alloc)
		return bp; //do nothing

	else if (prev_alloc && !next_alloc){
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
	}

	else if (!prev_alloc && next_alloc){
                size += GET_SIZE(FTRP(PREV_BLKP(bp)));
                PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
                PUT(FTRP(bp), PACK(size, 0));
		bp = PREV_BLKP(bp);
        }

	else{
		size += GET_SIZE(HDRP(NEXT_BLKP(bp))) + GET_SIZE(FTRP(PREV_BLKP(bp)));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
                PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
                bp = PREV_BLKP(bp);
	}

	return bp;
}

static void* find_fit(size_t);
static void place(void*, size_t);
// allocate size bytes of payload, return newly allocated block pointer, NULL on error
void* mm_malloc(size_t size){
	size_t asize; // adjusted size(payload+overheap+padding)
	size_t extendsize; //heap extension if no fit
	char* bp;

	//ignore spurious request
	if (size == 0)
		return NULL;

	// block size adjustment
	size = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

	// search the free list for a fit
	if (((bp = find_fit(asize)) != NULL)){
		place(bp, asize);
		return bp;
	}

	// no fit found, get more heap memory from memory system and place the block
	extendsize = MAX(asize, CHUNKSIZE);
	if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
		return NULL;
	place(bp, asize);
	return bp;
}

// first-fit search of block of size bytes, return NULL if not found
static void* find_fit(size_t size){ //block size
	//static char* heap_listp;
        void* current_bp = heap_listp;
	while (!GET_ALLOC(HDRP(current_bp)) && (GET_SIZE(HDRP(current_bp)) < size)){
		current_bp = NEXT_BLKP(current_bp);
		// if current block is epilogue block
		if ((GET_SIZE(HDRP(current_bp))==0))
			//search end, no fit
			return NULL;
	}
	return current_bp;
}

// place the adjusted block of size bytes at bp, split if remainder >= 2*DSIZE
static void place(void* bp, size_t size){
	size_t old_size = GET_SIZE(HDRP(bp));
	size_t rem_size = old_size - size;

	if (rem_size >= 2*DSIZE){ //split
		PUT(HDRP(bp), PACK(size, 1));
		PUT(FTRP(bp), PACK(size, 1));
		//new free block
		PUT(HDRP(NEXT_BLKP(bp)), PACK(rem_size, 0));
                PUT(FTRP(NEXT_BLKP(bp)), PACK(rem_size, 0));
	}

	else{ // place the requested block
        	PUT(HDRP(bp), PACK(old_size, 1));
        	PUT(FTRP(bp), PACK(old_size, 1));
	}
}









