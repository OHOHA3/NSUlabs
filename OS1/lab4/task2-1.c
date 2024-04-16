#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	printf("pid: %d\n", getpid());
	sleep(1);
	execl("./a.out", "a", NULL);
	printf("Hello world!");
	return 0;
}