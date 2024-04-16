#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct _Node {
	char value[100];
	struct _Node* next;
	pthread_rwlock_t sync;
} Node;

typedef struct _List {
	Node* first;
} List;

List* list_init(int size);
Node* node_init();
void list_destroy(List *l);
int list_get(Node *n);
void shuffle(Node *n);