#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>

#define HEAP_SIZE 40960
#define HEAP_ADD 0x20000000
#define META_SIZE 16

void* my_malloc(int size);
void my_free(void* p);

typedef struct buffer {
	int size;
	struct buffer* next;
}Buffer;

Buffer* heap = NULL;

int main() {
	mmap((char*)HEAP_ADD, HEAP_SIZE, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	int* p = (int*)my_malloc(10 * sizeof(int));
	for (int i = 0; i < 10; i++) {
		p[i] = i;
	}
	printf("First malloc\n");
	for (int i = 0; i < 10; i++) {
		printf("%d address :%p\n", i, p + i);
		printf("%d value   :%d\n", i, p[i]);
	}

	printf("Second malloc\n");
	int* pp = (int*)my_malloc(4 * sizeof(int));
	printf("%p\n", pp);

	printf("free first\n");
	my_free(p);
	int* ppp = (int*)my_malloc(sizeof(int));
	printf("%p\n", ppp);

	printf("big malloc\n");
	int* pppp = (int*)my_malloc(1000 * sizeof(int));
	printf("%p\n", pppp);
	return 0;
}

void* my_malloc(int size) {
	if (!heap) {
		heap = (Buffer*)HEAP_ADD;
		heap->next = NULL;
		heap->size = size;
		return (void*)((long long)heap + META_SIZE);
	}
	if ((long long)(heap) - HEAP_ADD > size + META_SIZE) {
		Buffer* new_buf = (Buffer*)HEAP_ADD;
		new_buf->size = size;
		new_buf->next = heap;
		heap = new_buf;
		return (void*)((long long)new_buf + META_SIZE);
	}
	Buffer* buf = heap;
	int mem_space;
	while (buf && buf->next) {
		mem_space = (long long)buf->next - META_SIZE - (long long)buf - buf->size;
		if (mem_space > size + META_SIZE) {
			Buffer* new_buf = (void*)((long long)buf + buf->size + META_SIZE);
			new_buf->size = size;
			new_buf->next = NULL;
			buf->next = new_buf;
			return (void*)((long long)new_buf + META_SIZE);
		}
		buf = buf->next;
	}
	if (HEAP_ADD + HEAP_SIZE - (long long)buf - buf->size - META_SIZE > size + META_SIZE) {
		Buffer* new_buf = (void*)((long long)buf + buf->size + META_SIZE);
		new_buf->size = size;
		new_buf->next = buf->next;
		buf->next = new_buf;
		return (void*)((long long)new_buf + META_SIZE);
	}
	else {
		return 0;
	}
}

void my_free(void* p) {
	if (heap == p - META_SIZE) {
		heap = heap->next;
		return;
	}
	Buffer* buf = heap;
	while (buf) {
		if (buf->next != NULL) {
			if (buf->next == p - META_SIZE) {
				buf->next = buf->next->next;
				return;
			}
		}
		buf = buf->next;
	}
	printf("invalid pointer\n");
	exit(0);
}
