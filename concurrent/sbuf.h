typedef struct {
	int* buf;		// buffer array
	int n;			// max number of slots
	int front;		// buf[(front+1)%n] is the first inserted item
	int rear;		// buf[rear%n] is the last inserted item
	sem_t mutex;	// protect access to buf
	sem_t slots;	// available slots
	sem_t items;	// available items
} sbuf_t;

void sbuf_init(sbuf_t* sp, int n);
void sbuf_deinit(sbuf_t* sp);
void sbuf_insert(sbuf_t* sp, int item);
int sbuf_remove(sbuf_t* sp);
