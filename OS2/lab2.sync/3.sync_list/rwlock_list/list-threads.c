#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "list.h"

List* l;

int asc_iteration = 0;
int desc_iteration = 0;
int sim_iteration = 0;
int frst_shuffler_iter = 0;
int scnd_shuffler_iter = 0;
int thrd_shuffler_iter = 0;

void *ascent_checker(void *arg) {
	while (1) {
		int cnt = 0;
		Node* n = l->first;
		pthread_rwlock_rdlock(&n->sync);
		while (n->next != NULL) {
			int len1 = list_get(n);
			Node *nn = n->next;
			pthread_rwlock_rdlock(&nn->sync);
			pthread_rwlock_unlock(&n->sync);
			int len2 = list_get(nn);
			if (len1 < len2) {
				cnt++;
			}
			n = nn;
		}
		pthread_rwlock_unlock(&n->sync);
		asc_iteration++;
		printf("ascent_checker: %d ascents, %d iterations\n", cnt, asc_iteration);
	}

	return NULL;
}

void *descent_checker(void *arg) {
	while (1) {
		int cnt = 0;
		Node* n = l->first;
		pthread_rwlock_rdlock(&n->sync);
		while (n->next != NULL) {
			int len1 = list_get(n);
			Node* nn = n->next;
			pthread_rwlock_rdlock(&nn->sync);
			pthread_rwlock_unlock(&n->sync);
				int len2 = list_get(nn);
			if (len1 > len2) {
				cnt++;
			}
			n = nn;
		}
		pthread_rwlock_unlock(&n->sync);
		desc_iteration++;
		printf("descent_checker: %d descents, %d iterations\n", cnt, desc_iteration);
	}

	return NULL;
}

void* similar_checker(void* arg) {
	while (1) {
		int cnt = 0;
		Node* n = l->first;
		pthread_rwlock_rdlock(&n->sync);
		while (n->next != NULL) {
			int len1 = list_get(n);
			Node* nn = n->next;
			pthread_rwlock_rdlock(&nn->sync);
			pthread_rwlock_unlock(&n->sync);
				int len2 = list_get(nn);
			if (len1 == len2) {
				cnt++;
			}
			n = nn;
		}
		pthread_rwlock_unlock(&n->sync);
		sim_iteration++;
		printf("similar_checker: %d similars, %d iterations\n", cnt, sim_iteration);
	}

	return NULL;
}

void* shuffler(void* arg) {
	unsigned int seed = time(NULL) ^ pthread_self();
	
	int* iterations = (int*)arg;
	
	while (1) {
		int cnt = 0;
		Node* n1 = l->first;
		pthread_rwlock_wrlock(&n1->sync);
		Node* n2 = n1->next;
		pthread_rwlock_wrlock(&n2->sync);
		while (n2->next != NULL) {
			Node* n3 = n2->next;
			pthread_rwlock_wrlock(&n3->sync);
			if (rand_r(&seed) % 100 < 5) {
				shuffle(n1);
				cnt++;
				pthread_rwlock_unlock(&n1->sync);
				n1 = n3;
			}
			else {
				pthread_rwlock_unlock(&n1->sync);
				n1 = n2;
				n2 = n3;
			}
		}
		pthread_rwlock_unlock(&n1->sync);
		pthread_rwlock_unlock(&n2->sync);
		(*iterations)++;
		printf("shuffler: %d shuffles, %d iterations\n", cnt, *iterations);
	}

	return NULL;
}

int main() {
	pthread_t tid;
	int err;

	l = list_init(10000);

	err = pthread_create(&tid, NULL, ascent_checker, NULL);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	err = pthread_create(&tid, NULL, descent_checker, NULL);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	err = pthread_create(&tid, NULL, similar_checker, NULL);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	err = pthread_create(&tid, NULL, shuffler, &frst_shuffler_iter);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	err = pthread_create(&tid, NULL, shuffler, &scnd_shuffler_iter);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	err = pthread_create(&tid, NULL, shuffler, &thrd_shuffler_iter);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	sleep(10);

	return 0;
}
