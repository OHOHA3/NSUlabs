#include <unistd.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <stdlib.h>

int main(int argc, char **argv){
	pid_t pid = 0;
	pid = fork();
	if (!pid){
		if (argc == 1){
			printf("No arguments\n");
			exit(1);
		}
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		int check = 0;
		check = execvp(argv[1],argv+1);
		if (check == -1){
			perror("execvp");
			exit(1);
		}
	}
	else{
	  	struct user_regs_struct state;
		int status;
		int cnt = 0;
		wait(&status);
		ptrace(PTRACE_GETREGS, pid, 0, &state);
		printf("SYSCALL %lld\n", state.orig_rax);
		cnt++;
		while (!WIFEXITED(status)){
		  	ptrace(PTRACE_SYSCALL, pid, 0, 0);
			wait(&status);
			if (WIFSTOPPED(status)) {
				ptrace(PTRACE_GETREGS, pid, 0, &state);
				printf("SYSCALL %lld\n", state.orig_rax);
				cnt++;
				ptrace(PTRACE_SYSCALL, pid, 0, 0);
				wait(&status);
			}
		}
		printf("Number of syscalls: %d\n",cnt);
	}
	return 0;
}
