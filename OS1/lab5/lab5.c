#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int global_var = 5;

int main() {
	int local_var = 7;
	printf("global var add: %p\n", &global_var);
	printf("global var val: %d\n", global_var);
	printf("local var add : %p\n", &local_var);
	printf("local var val : %d\n", local_var);
	printf("pid: %d\n", getpid());
	int pid = fork();
	if (pid == 0) {
		printf("child pid :  %d\n", getpid());
		printf("parent pid: %d\n", getppid());
		printf("child - global var add: %p\n", &global_var);
		printf("child - global var val: %d\n", global_var);
		printf("child - local var add : %p\n", &local_var);
		printf("child - local var val : %d\n", local_var);
		global_var = 50;
		local_var = 70;
		printf("child - global changed var val: %d\n", global_var);
		printf("child - local changed var val : %d\n", local_var);
		//sleep(30);
		exit(5);
	}
	if (pid > 0) {
		printf("parent - global var val: %d\n", global_var);
		printf("parent - local var val : %d\n", local_var);
		sleep(30);
		int status;
		wait(&status);
		if (WIFEXITED(status)) {
			printf("ended successfully\n");
			printf("code - %d\n", WEXITSTATUS(status));
		}
		if (WIFSIGNALED(status)) {
			printf("ended with signal\n");
			printf("code - %d\n", WTERMSIG(status));
		}
		if (WIFSTOPPED(status)) {
			printf("stopped with signal\n");
			printf("code - %d\n", WSTOPSIG(status));
		}
	}
	if (pid == -1) {
		perror("fork");
		exit(0);
	}
	return 0;
}