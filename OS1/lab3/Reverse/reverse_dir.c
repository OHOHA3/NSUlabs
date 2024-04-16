#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char* make_reverse_dir(char* dir_path, int* len);
void make_reverse_files(int dir_name_len, char* dir_name, char* dir_path);

int main(int argc, char** argv){
	if (argc == 1){
		printf("too few arguments\n");
		return 0;
	}
	if (argc > 2) {
		printf("too many arguments\n");
		return 0;
	}
	int dir_name_len;
	char* dir_name = make_reverse_dir(argv[1], &dir_name_len);
	make_reverse_files(dir_name_len, dir_name, argv[1]);
	return 0;
}

char* make_reverse_dir(char* dir_path, int* len){
	int dir_name_len = 0;
	char* base_name = basename(dir_path);
	for (int i = 0; base_name[i]; i++){
		dir_name_len++;
	}
	char* dir_name = (char*)calloc(dir_name_len, sizeof(char));
	for (int i = 0; i < dir_name_len; i++){
		dir_name[i] = base_name[dir_name_len - 1 - i];
	}
	int check;
	check = mkdir(dir_name, 0777);
	if (check == -1){
		perror("mkdir");
		exit(0);
	}
	*len = dir_name_len;
	return dir_name;
}

void make_reverse_files(int dir_name_len, char* dir_name, char* dir_path){
	DIR* dir;
	FILE* input, * output;
	struct dirent* entry;
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
		int file_name_len = 0;
		for (int i = 0; entry->d_name[i]; i++){
			file_name_len++;
		}
		for (int i = 0; i < dir_name_len; i++){
			file_name[i] = dir_name[i];
		}
		file_name[dir_name_len] = '/';
		for (int i = 1; i <= file_name_len; i++){
			file_name[dir_name_len + i] = entry->d_name[file_name_len - i];
		}
		output = fopen(file_name, "wb");
		if (output == NULL){
			perror("fopen");
			exit(0);
		}
		char* exec_dir;
		int val;
		val = asprintf(&exec_dir, "%s/%s", dir_path, entry->d_name);
		if (val == -1){
			perror("asprintf");
			exit(0);
		}
		input = fopen(exec_dir, "rb");
		if (input == NULL){
			perror("fopen");
			exit(0);
		}
		int offset = 1;
		char sym;
		fseek(input, 0, SEEK_END);
		while (!fseek(input, -offset, SEEK_CUR)){
			fread(&sym, sizeof(char), 1, input);
			fwrite(&sym, sizeof(char), 1, output);
			offset = 2;
		}
		fclose(input);
		fclose(output);
		free(exec_dir);
	}
	free(dir_name);
	int check;
	check = closedir(dir);
	if (check == -1){
		perror("closedir");
		exit(0);
	}
}