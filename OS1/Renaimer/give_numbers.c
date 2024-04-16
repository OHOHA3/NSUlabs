#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

char* copy_dir(char* dir_path);
void copy_files(char* dir_name, char* dir_path);
int* shuffle_numbers(char* dir_path);
void make_file_name(char* dir_name, int numb, char* file_name, char* inner_file_name);
void make_order(int numb, char* file_name, int* file_name_len);
void copy_file(char* file_name, char* dir_path, char* inner_file_name);

int main(int argc, char** argv){
	if (argc == 1){
		printf("too few arguments\n");
		return 0;
	}
	if (argc > 2){
		printf("too many arguments\n");
		return 0;
	}
	printf("Start renaiming\n");
	char* dir_name = copy_dir(argv[1]);
	copy_files(dir_name, argv[1]);
	printf("\nRenaiming completed successful\n");
	return 0;
}

char* copy_dir(char* dir_path){
	int dir_name_len = 0;
	char* base_name = basename(dir_path);
	for (int i = 0; base_name[i]; i++){
		dir_name_len++;
	}
	char* dir_name = (char*)calloc(dir_name_len, sizeof(char));
	for (int i = 0; base_name[i]; i++) {
		dir_name[i] = base_name[i];
	}
	int check;
	check = mkdir(dir_name, 0777);
	if (check == -1){
		perror("mkdir");
		exit(0);
	}
	return dir_name;
}

void copy_files(char* dir_name, char* dir_path){
	DIR* dir;
	struct dirent* entry;
	int* name_numbers = shuffle_numbers(dir_path);
	int count = 0;
	dir = opendir(dir_path);
	if (!dir){
		perror("opendir");
		exit(0);
	}
	while ((entry = readdir(dir)) != NULL){
		if (entry->d_type != DT_REG){
			continue;
		}
		char file_name[512] = { 0 };
		make_file_name(dir_name, name_numbers[count], file_name, entry->d_name);
		copy_file(file_name, dir_path, entry->d_name);
		count++;
		if (count % 50 == 0){
			printf("\nFiles renamed : %d\n\n", count);
		}
	}
	free(dir_name);
	int check;
	check = closedir(dir);
	if (check == -1){
		perror("closedir");
		exit(0);
	}
}

int* shuffle_numbers(char* dir_path) {
	DIR* dir;
	struct dirent* entry;
	int count = 0;
	dir = opendir(dir_path);
	int* name_numbers;
	if (!dir) {
		perror("opendir");
		exit(0);
	}
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_type != DT_REG){
			continue;
		}
		count++;
	}
	printf("\nTotal files: %d\n\n", count);
	name_numbers = (int*)malloc(count * sizeof(int));
	for (int i = 0; i < count; i++) {
		name_numbers[i] = i + 1;
	}
	srand(time(NULL));
	int index;
	int tmp;
	for (int i = 0; i < count; i++) {
		index = rand() % count;
		tmp = name_numbers[index];
		name_numbers[index] = name_numbers[i];
		name_numbers[i] = tmp;
	}
	int check = closedir(dir);
	if (check == -1) {
		perror("closedir");
		exit(0);
	}
	return name_numbers;
}

void make_file_name(char* dir_name, int numb, char* file_name, char* inner_file_name) {
	int file_name_len = 0;
	for (int i = 0; dir_name[i]; i++) {
		file_name[file_name_len++] = dir_name[i];
	}
	file_name[file_name_len++] = '/';

	make_order(numb, file_name, &file_name_len);

	for (int i = 0; inner_file_name[i]; i++) {
		file_name[file_name_len++] = inner_file_name[i];
		printf("%c", inner_file_name[i]);
	}
	printf("\n");
}

void make_order(int numb, char* file_name, int* file_name_len) {
	int len = *file_name_len;
	//file_name[len++] = '[';
	char numbs[3];
	for (int i = 0; i < 3; i++) {
		numbs[i] = numb % 10 + '0';
		numb /= 10;
	}
	//int old_len = len;
	for (int i = 2; i >= 0; i--) {
		//if (numbs[i] != '0' || old_len != len){
		file_name[len++] = numbs[i];
		//}
	}
	//file_name[len++] = ']';
	file_name[len++] = ' ';
	//file_name[len++] = '-';
	//file_name[len++] = ' ';
	*file_name_len = len;
}

void copy_file(char* file_name, char* dir_path, char* inner_file_name) {
	FILE *input, *output;
	output = fopen(file_name, "wb");
	if (output == NULL) {
		perror("fopen");
		exit(0);
	}
	char* exec_dir;
	int val;
	val = asprintf(&exec_dir, "%s/%s", dir_path, inner_file_name);
	if (val == -1) {
		perror("asprintf");
		exit(0);
	}
	input = fopen(exec_dir, "rb");
	if (input == NULL) {
		perror("fopen");
		exit(0);
	}
	char buffer[500000];
	int read_part;
	do {
		read_part = fread(buffer, sizeof(char), 500000, input);
		fwrite(buffer, sizeof(char), read_part, output);
	} while (read_part != 0);
	fclose(input);
	fclose(output);
	free(exec_dir);
}