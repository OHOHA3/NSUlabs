#define _GNU_SOURCE
#include <pthread.h>

#include "list.h"

List* list_init(int max_count) {

	List *l = malloc(sizeof(List));
	if (!l) {
		printf("Cannot allocate memory for a list\n");
		abort();
	}
	
	Node *n = node_init();
	
	l->first = n;
	
	Node* nn;
	for (int i = 1; i < max_count; i++) {
		nn = node_init();
		n->next = nn;
		n = nn;
	}
	
	return l;
}

Node* node_init() {
	Node *n = malloc(sizeof(Node));
	if (!n) {
		printf("Cannot allocate memory for a list\n");
		abort();
	}
	
	int len = rand() % 100;
	for (int i = 0; i < len; i++) {
		n->value[i] = '7';
	}
	
	n->next = NULL;

	pthread_rwlock_init(&n->sync, NULL);

	return n;
}

void list_destroy(List *l) {
	Node* node = l->first;

	while (node != NULL) {
		Node* tmp = node->next;
		free(node);
		node = tmp;
	}

	free(l);

	printf("list has destroyed successfully\n");
}

int list_get(Node *n) {
	int len = strlen(n->value);
	return len;
}

void shuffle(Node* n) {
	Node* n2 = n->next;
	Node* n3 = n2->next;

	n2->next = n3->next;
	n3->next = n2;
	n->next = n3;
}