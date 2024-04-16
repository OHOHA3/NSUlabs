#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int my_global_not_inited;
int my_global_inited = 10;

const int my_global_const = 20;

void make_var_in_func();
void work_with_heap();
void work_with_env();
int* get_local_addr();

int main() {
	printf("global inited:     %p\n", &my_global_inited);
	printf("global not inited: %p\n", &my_global_not_inited);
	printf("global const:      %p\n", &my_global_const);
	make_var_in_func();
	printf("pid: %d\n", getpid());

	//work_with_heap();
	//printf("%p\n", get_local_addr());
	//work_with_env();
	//sleep(30);
}

void make_var_in_func() {
	int my_local = 1;
	static int my_local_static = 2;
	const int my_local_const = 3;
	printf("local:             %p\n", &my_local);
	printf("local static:      %p\n", &my_local_static);
	printf("local const:       %p\n", &my_local_const);
}

void work_with_heap() {
	char* buf1 = (char*)malloc(sizeof(char) * 15);
	sprintf(buf1, "%s", "hello world!");
	printf("%s\n", buf1);
	free(buf1);
	printf("%s\n", buf1);

	char* buf2 = (char*)malloc(sizeof(char) * 15);
	sprintf(buf2, "%s", "hello world!");
	printf("%s\n", buf2);
	buf2 += 5 * sizeof(char);
	printf("%s\n", buf2);
	free(buf2);
	printf("%s\n", buf2);
}

void work_with_env() {
	char* my_env;
	if (my_env = getenv("my_env")) {
		printf("%s\n", my_env);
	}
	else {
		printf("this env doesn't exist\n");
		exit(0);
	}
	if (setenv("my_env", "22", 1) == -1) {
		perror("setenv");
		exit(0);
	}
	if (my_env = getenv("my_env")) {
		printf("%s\n", my_env);
	}
	else {
		printf("this env doesn't exist\n");
		exit(0);
	}
}

int* get_local_addr() {
	int var = 5;
	return &var;
}
+