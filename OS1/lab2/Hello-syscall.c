#include <unistd.h>
#include <errno.h>
#include <sys/syscall.h>
#include <stdio.h>

static ssize_t my_write(int fd, void* buf, size_t count) {
	return syscall(SYS_write, fd, buf, count);
}

int main() {
	int check = 0;
	check = my_write(1, "Hello with syscall!\n", 21);
	if (check == -1) {
		perror("");
	}
	return 0;
}