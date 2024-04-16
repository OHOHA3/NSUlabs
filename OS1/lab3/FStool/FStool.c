#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void execute(char* prog, char* path, char* arg);
void make_dir(char* name);
void print_dir(char* name);
void remove_dir(char* name);
void make_file(char* name);
void print_file(char* name);
void remove_file(char* name);
void make_symlink(char* name, char* link_name);
void print_symlink(char* name);
void print_linkfile(char* name);
void remove_symlink(char* name);
void make_hardlink(char* name, char* link_name);
void remove_hardlink(char* name);
void print_permission(char* name);
void set_permission(char* name, char* mode);

int main(int argc, char** argv) {
	if (argc > 3) {
		printf("too many arguments\n");
		return 0;
	}
	if (argc == 1) {
		printf("too few arguments\n");
		return 0;
	}
	if (argc == 2)
		execute(argv[0], argv[1], NULL);
	else
		execute(argv[0], argv[1],argv[2]);
	return 0;
}

void execute(char* prog, char* path, char* arg) {
	char* name = basename(prog);
	if (!strcmp(name, "make_dir"))
		make_dir(path);
	else
	if (!strcmp(name, "print_dir"))
		print_dir(path);
	else
	if (!strcmp(name, "remove_dir"))
		remove_dir(path);
	else
	if (!strcmp(name, "make_file"))
		make_file(path);
	else
	if (!strcmp(name, "print_file"))
		print_file(path);
	else
	if (!strcmp(name, "remove_file"))
		remove_file(path);
	else
	if (!strcmp(name, "make_symlink"))
		make_symlink(path, arg);
	else
	if (!strcmp(name, "print_symlink"))
		print_symlink(path);
	else
	if (!strcmp(name, "print_linkfile"))
		print_linkfile(path);
	else
	if (!strcmp(name, "remove_symlink"))
		remove_symlink(path);
	else
	if (!strcmp(name, "make_hardlink"))
		make_hardlink(path, arg);
	else
	if (!strcmp(name, "remove_hardlink"))
		remove_hardlink(path);
	else
	if (!strcmp(name, "print_permission"))
		print_permission(path);
	else
	if (!strcmp(name, "set_permission"))
		set_permission(path, arg);
	else {
		printf("invalid program\n");
		exit(0);
	}
}

void make_dir(char* name) {
	if (mkdir(name, 0777) == -1) {
		perror("mkdir");
		exit(0);
	}
}

void print_dir(char* name) {
	DIR* dir;
	struct dirent* entry;
	dir = opendir(name);
	if (!dir) {
		perror("opendir");
		exit(0);
	}
	while ((entry = readdir(dir)) != NULL) {
		printf("name - [%s] type - %d\n", entry->d_name, entry->d_type);
	}
	if (closedir(dir) == -1) {
		perror("closedir");
		exit(0);
	}
}

void remove_dir(char* name) {
	DIR* dir;
	struct dirent* entry;
	dir = opendir(name);
	if (!dir) {
		perror("opendir");
		exit(0);
	}
	char* full_name;
	while ((entry = readdir(dir)) != NULL) {
		asprintf(&full_name, "%s/%s", name, entry->d_name);
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}
		if (entry->d_type == DT_DIR) {
			remove_dir(full_name);
		}
		else {
			if (remove(full_name) == -1) {
				printf("%s\n", full_name);
				perror("remove");
				exit(0);
			}
		}
		memset(full_name, '0', strlen(full_name));
	}
	if (remove(name) == -1) {
		perror("remove");
		exit(0);
	}
	free(full_name);
	if (closedir(dir) == -1) {
		perror("closedir");
		exit(0);
	}
}

void make_file(char* name) {
	int fd = open(name, O_CREAT | O_EXCL, 0777);
	if (fd == -1) {
		perror("open");
		exit(0);
	}
	close(fd);
}

void print_file(char* name) {
	int fd = open(name, O_RDONLY);
	if (fd == -1) {
		perror("open");
		exit(0);
	}
	char buf;
	while (read(fd, &buf, sizeof(char))) {
		printf("%c", buf);
	}
	printf("\n");
}

void remove_file(char* name) {
	if (remove(name) == -1) {
		perror("remove");
		exit(0);
	}
}

void make_symlink(char* name, char* link_name) {
	if (symlink(name, link_name) == -1) {
		perror("symlink");
		exit(0);
	}
}

void print_symlink(char* name) {
	char buf[1000] = { 0 };
	if (readlink(name, buf, 1000) == -1) {
		perror("readlink");
		exit(0);
	}
	printf("%s\n", buf);
}

void print_linkfile(char* name) {
	print_file(name);
}

void remove_symlink(char* name) {
	remove_file(name);
}

void make_hardlink(char* name, char* link_name) {
	if (link(name, link_name) == -1) {
		perror("symlink");
		exit(0);
	}
}

void remove_hardlink(char* name) {
	remove_file(name);
}

void print_permission(char* name) {
	struct stat file;
	if (stat(name, &file) == -1) {
		perror("stat");
		exit(0);
	}
	printf("permission - %04o\nHardlinks - %ld\n", file.st_mode, file.st_nlink);
}

void set_permission(char* name, char* mode) {
	if (strlen(mode) != 3) {
		printf("invalid mode\n");
		exit(0);
	}
	for (int i = 0; i < 3; i++) {
		if (mode[i] > '7' || mode[i] < '0') {
			printf("invalid mode\n");
			exit(0);
		}
	}
	int number;
	number = (mode[0] - '0') * 64 + (mode[1] - '0') * 8 + mode[2] - '0';
	if (chmod(name, number) == -1) {
		perror("chmod");
		exit(0);
	}
}