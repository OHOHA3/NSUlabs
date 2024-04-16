#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int main() {
	int check = 0;
	check = write(1, "Hello with write!\n", 19);
	if (check == -1) {
		perror("");
	}
	return 0;
}
