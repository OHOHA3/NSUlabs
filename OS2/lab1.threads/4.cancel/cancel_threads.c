#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <malloc.h>

void* print_data(void* arg) {
	while (1) {
		printf("hello world\n");
	}
	return NULL;
}

void* increment(void* arg) {
	int i = 0;
	while (1) {
		i++;
		pthread_testcancel();
	}
	return NULL;
}

void my_free(void* arg) {
	printf("free routine\n");
	free(arg);
}

void* malloc_data(void* arg) {
	char* word = (char*)malloc(sizeof("hello"));
	strcpy(word, "hello");
	pthread_cleanup_push(my_free, word);
	while (1) {
		printf("%s\n", word);
		sleep(1);
	}
	pthread_cleanup_pop(1);
	return NULL;
}

int main() {
	pthread_t tid;
	int err;
	err = pthread_create(&tid, NULL, print_data, NULL);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
	pthread_cancel(tid);

	err = pthread_create(&tid, NULL, increment, NULL);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
	sleep(3);
	pthread_cancel(tid);

	err = pthread_create(&tid, NULL, malloc_data, NULL);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
	sleep(3);
	pthread_cancel(tid);

	pthread_exit(NULL);
	return 0;
}