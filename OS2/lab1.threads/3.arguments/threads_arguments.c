#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <malloc.h>

typedef struct data {
	int number;
	char* word;
}Data;

void* print_data(void* arg) {
	Data* data = (Data*)arg;
	printf("%d\n", data->number);
	printf("%s\n", data->word);
	return NULL;
}

int main() {
	Data* data = (Data*)malloc(sizeof(Data));
	data->number = 1;
	data->word = "hello";

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_t tid;
	int err;
	err = pthread_create(&tid, &attr, print_data, data);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	pthread_exit(NULL);
	return 0;
}