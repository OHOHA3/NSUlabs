#define MAX_THREADS 8
#define STACK_SIZE 8 * 1024 * 1024

typedef struct uthread_meta {
	int uthread_count;
	int uthread_cur;
} uthread_meta_t;

typedef struct uthread {
	void (*thread_func)(void*);
	void* arg;
	ucontext_t uctx;
	uthread_meta_t* meta;
} uthread_t;

void uthread_scheduler(void);
void start_thread(void);
void uthread_create(uthread_t** ut, void (*thread_func)(void*), void* arg);
void uthread_init(uthread_t* ut);