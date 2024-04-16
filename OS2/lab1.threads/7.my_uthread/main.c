#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include "my_uthread.h"

int iter = 0;

void mythread1(void* arg) {
	for (int i = 0; i < 5; i++) {
		iter++;
		printf("thread 1: iter - %d\n", iter);
		sleep(1);
		uthread_scheduler();
	}
	uthread_scheduler();
}

void mythread2(void* arg) {
	for (int i = 0; i < 5; i++) {
		iter++;
		printf("thread 2: iter - %d\n", iter);
		sleep(1);
		uthread_scheduler();
	}
	uthread_scheduler();
}

void mythread3(void* arg) {
	for (int i = 0; i < 5; i++) {
		iter++;
		printf("thread 3: iter - %d\n", iter);
		sleep(1);
		uthread_scheduler();
	}
	uthread_scheduler();
}

int main() {
	uthread_t* ut[3];

	uthread_t main_thread;
	uthread_init(&main_thread);

	uthread_create(&ut[0], mythread1, NULL);
	uthread_create(&ut[1], mythread2, NULL);
	uthread_create(&ut[2], mythread3, NULL);

	while (1) {
		uthread_scheduler();
	}

	printf("main: finished\n");
	return 0;
}