#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void* get_number(void* arg) {
	sleep(1);
	static int number = 42;
	return &number;
}

void* get_word(void* arg) {
	sleep(1);
    char* word = "hello world";
	return word;
}

int main() {
	pthread_t tid;
	int err;

	err = pthread_create(&tid, NULL, get_number, NULL);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
	void* number;
	pthread_join(tid, &number);
	printf("%d\n", *(int*)number);

	err = pthread_create(&tid, NULL, get_word, NULL);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
	void* word;
	pthread_join(tid, &word);
	printf("%s\n", (char*)word);

	return 0;
}