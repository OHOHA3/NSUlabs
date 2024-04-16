#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include "my_uthread.h"

uthread_t* uthreads[MAX_THREADS] = { 0 };

void uthread_scheduler(void) {
	int err;
	ucontext_t* cur_ctx, * next_ctx;

	int cur_thread = uthreads[0]->meta->uthread_cur;
	int val_thread = uthreads[0]->meta->uthread_count;
	cur_ctx = &(uthreads[cur_thread]->uctx);
	int next_thread = (cur_thread + 1) % val_thread;
	next_ctx = &(uthreads[next_thread]->uctx);
	uthreads[0]->meta->uthread_cur = next_thread;
	err = swapcontext(cur_ctx, next_ctx);
	if (err == -1) {
		printf("uthread_scheduler error: %s\n", strerror(errno));
		exit(1);
	}
}

void start_thread(void) {
	ucontext_t* ctx;
	int val_thread = uthreads[0]->meta->uthread_count;
	for (int i = 0; i < val_thread; i++) {
		ctx = &uthreads[i]->uctx;
		char* stack_from = ctx->uc_stack.ss_sp;
		char* stack_to = ctx->uc_stack.ss_sp + ctx->uc_stack.ss_size;

		if (stack_from <= (char*)&i && (char*)&i <= stack_to) {
			uthreads[i]->thread_func(uthreads[i]->arg);
		}
	}
}

void uthread_create(uthread_t** ut, void (*thread_func)(void*), void* arg) {
	char* stack;
	uthread_t* new_ut;
	int err;

	stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
	memset(stack, 0x0, STACK_SIZE);
	new_ut = (uthread_t*)(stack + STACK_SIZE - sizeof(uthread_t));

	err = getcontext(&new_ut->uctx);
	if (err == -1) {
		printf("uthread_create error: %s\n", strerror(errno));
		exit(1);
	}

	new_ut->uctx.uc_stack.ss_sp = stack;
	new_ut->uctx.uc_stack.ss_size = STACK_SIZE - sizeof(uthread_t);
	new_ut->uctx.uc_link = NULL;
	makecontext(&new_ut->uctx, start_thread, 0);

	new_ut->thread_func = thread_func;
	new_ut->arg = arg;

	int val_thread = uthreads[0]->meta->uthread_count;
	uthreads[0]->meta->uthread_count++;
	uthreads[val_thread] = new_ut;
	
	*ut = new_ut;
}

void uthread_init(uthread_t* ut) {
	uthreads[0] = ut;
	uthreads[0]->meta = (uthread_meta_t*)malloc(sizeof(uthread_meta_t));
	uthreads[0]->meta->uthread_count = 1;
	uthreads[0]->meta->uthread_cur = 0;
}