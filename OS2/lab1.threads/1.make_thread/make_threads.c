#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int global = 1;

void *mythread(void *arg) {
	int local = 2;
	static int stat = 3;
	const int con = 4;
	printf("global address: %p\n", &global);
	printf("local address: %p\n", &local);
	printf("static address: %p\n", &stat);
	printf("const address: %p\n", &con);
	printf("mythread [%d %d %d %ld]: Hello from mythread!\n", getpid(), getppid(), gettid(), pthread_self());
	return NULL;
}

void* check_var(void* arg) {
	printf("change var\n");
	int local = 0;
	local++;
	global++;
	printf("check var\n");
	printf("global: %d\n", global);
	printf("local: %d\n", local);
	return NULL;
}

int main() {
	pthread_t tid;
	int err;

	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
	int thread = 0;
	for(int i = 0;i<3;i++){
		err = pthread_create(&tid, NULL, mythread, NULL);
		int new_thread = tid;
		printf("%d \n", pthread_equal(thread, new_thread));
		printf("%ld:\n",tid);
		if (err) {
	    	printf("main: pthread_create() failed: %s\n", strerror(err));
			return -1;
		}
		thread = new_thread;
	}
	for (int i = 0; i < 3; i++) {
		pthread_create(&tid, NULL, check_var, NULL);
	}
	sleep(30);
	pthread_exit(NULL);
	return 0;
}

