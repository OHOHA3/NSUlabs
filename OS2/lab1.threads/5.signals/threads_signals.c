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

void* block_signals(void* arg) {
	sigset_t signal_set;
	sigfillset(&signal_set);
	sigprocmask(SIG_BLOCK, &signal_set, NULL);
	while (1) {
		printf("block all signals\n");
		sleep(1);
	}
	return NULL;
}

void SIGINT_handler(int signum) {
	printf("catch SIGINT\n");
}

void* SIGINT_catcher(void* arg) {
	sigset(SIGINT, SIGINT_handler);
	while (1) {
		sleep(1);
	}
	return NULL;
}

void* SIGQUIT_catcher(void* arg) {
	sigset_t signal_set;
	sigemptyset(&signal_set);
	sigaddset(&signal_set, SIGQUIT);
	int signal;
	while (1) {
		sigwait(&signal_set, &signal);
		if (signal == SIGQUIT) {
			printf("catch SIGQUIT\n");
		}
	}
	return NULL;
}

int main() {
	sigset_t signal_set;
	sigfillset(&signal_set);
	sigprocmask(SIG_BLOCK, &signal_set, NULL);

	pthread_t tid1;
	pthread_t tid2;
	pthread_t tid3;
	int err;
	err = pthread_create(&tid1, NULL, block_signals, NULL);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
	
	err = pthread_create(&tid2, NULL, SIGINT_catcher, NULL);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	err = pthread_create(&tid3, NULL, SIGQUIT_catcher, NULL);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
	
	sleep(30);
	return 0;
}