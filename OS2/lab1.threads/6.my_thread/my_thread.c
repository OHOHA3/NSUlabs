#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/mman.h>

#define STACK_SIZE 8*1024*1024
#define PAGE 4096

typedef long long mythread_t;

typedef void* (*start_routine_t)(void*);

typedef struct _mythread {
	int mythread_id;
	start_routine_t start_routine;
	void* arg;
	void* retval;
	volatile int joined;
	volatile int exited;
} mythread_struct_t;

int mythread_startup(void* arg) {
	mythread_struct_t* mythread = (mythread_struct_t*)arg;

	mythread->retval = mythread->start_routine(mythread->arg);
	mythread->exited = 1;

	while (!mythread->joined) {
		sleep(1);
	}

	free(mythread);
	return 0;
}

int mythread_create(mythread_t* mytid, void*(*start_routine)(void*), void* arg) {
	static int thread_num = 0;
	mythread_struct_t* mythread;
	int child_pid;
	void* child_stack;

	thread_num++;

	child_stack = mmap(NULL, STACK_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	
	if ((long long)child_stack == -1) {
		printf("mythread_create error");
		return 1;
	}
	
	mprotect(child_stack + PAGE, STACK_SIZE - 2 * PAGE, PROT_READ | PROT_WRITE);
	
	memset(child_stack + PAGE, 0x0, STACK_SIZE - 2 * PAGE);

	mythread = (mythread_struct_t*)malloc(sizeof(mythread_struct_t));
	mythread->mythread_id = thread_num;
	mythread->start_routine = start_routine;
	mythread->arg = arg;
	mythread->retval = NULL;
	mythread->exited = 0;
	mythread->joined = 0;
	child_pid = clone(mythread_startup, child_stack + STACK_SIZE - PAGE, CLONE_VM | CLONE_SIGHAND | CLONE_FS | CLONE_FILES | CLONE_THREAD | CLONE_SYSVSEM | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID, (void*)mythread);
	if ((long long)child_pid == -1) {
		printf("mythread_create error");
		return 1;
	}

	*mytid = (mythread_t)mythread;

	return 0;
}

int mythread_join(mythread_t mytid, void** retval) {
	mythread_struct_t* mythread = (mythread_struct_t*)mytid;

	while (!mythread->exited) {
		sleep(1);
	}
	
	if (retval != NULL) {
		*retval = mythread->retval;
	}
	
	mythread->joined = 1;

	return 0;
}

typedef struct data {
	int number;
	char* word;
}Data;

void* print_data(void* arg) {
	Data* data = (Data*)arg;
	printf("%d\n", data->number);
	printf("%s\n", data->word);
	data->number = 10;
	data->word = "world";
	return data;
}

void* hello_first(void* arg) {
	while (1) {
		printf("Hello_first\n");
		sleep(1);
	}
}

void* hello_second(void* arg) {
	while (1) {
		printf("Hello_second\n");
		sleep(1);
	}
}

int main() {
	mythread_t tid;
	int err;

	err = mythread_create(&tid, hello_first, NULL);
	if (err) {
		printf("mythread_create error\n");
		return -1;
	}

	err = mythread_create(&tid, hello_second, NULL);
	if (err) {
		printf("mythread_create error\n");
		return -1;
	}

	Data* data = (Data*)malloc(sizeof(Data));
	data->number = 1;
	data->word = "hello";

	void* retval;

	err = mythread_create(&tid, print_data, data);
	if (err) {
		printf("mythread_create error\n");
		return -1;
	}
	mythread_join(tid,&retval);

	Data* answer = (Data*)retval;
	printf("%d\n", answer->number);
	printf("%s\n", answer->word);
	sleep(10);
	return 0;
}