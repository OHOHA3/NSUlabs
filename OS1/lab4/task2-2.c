#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <signal.h>

void work_with_stack();
void work_with_heap();
void work_with_mmap();
void work_with_prot();
void handler(int signum);
void work_with_munmap();

int main() {
	printf("%d\n", getpid());
	sleep(10);
	//work_with_stack(1);
	//work_with_heap();
	//work_with_mmap();
	//signal(SIGSEGV, handler);
	//work_with_prot();
	//work_with_munmap();
	return 0;
}

void work_with_stack(int iteration) {
	char buf[8192];
	printf("%d\n", 8192 * iteration);
	iteration++;
	usleep(50000);
	work_with_stack(iteration);
}

void work_with_heap() {
	long long it = 1;
	while(1) {
		char* buf = (char*)malloc(sizeof(char) * 8192);
		printf("%lld\n", 8192 * it);
		it++;
		usleep(50000);
	}
}

void work_with_mmap() {
	long long it = 1;
	while (1) {
		char* buf = (char*)malloc(sizeof(char) * 8192);
		if (it > 50) {
			mmap(NULL, 4096 * 10, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		}
		printf("%lld\n", 8192 * it);
		it++;
		usleep(50000);
	}
}

void work_with_prot() {
	mmap((char*)0x20000000, 4096, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	char* a = (char*)0x20000000;
	*a = 4;
	char b = *a;
}

void handler(int signum) {
	printf("Memory error\n");
	exit(0);
}

void work_with_munmap() {
	mmap((char*)0x20000000, 4096 * 10, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	sleep(5);
	munmap((char*)(0x20000000 + 4096 * 4), 4096 * 3);
	sleep(10);
}