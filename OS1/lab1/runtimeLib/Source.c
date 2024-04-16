#include <dlfcn.h>
#include <stdio.h>
int main() {
  void *library_handler;
  void (*print_hello)(void);
  library_handler = dlopen("./libhfdl.so", RTLD_LAZY);
  if (!library_handler){
	printf("dlopen() error: %s\n", dlerror());
	return 0;
}
  print_hello = dlsym(library_handler,"hello_from_dynamic_lib");
	print_hello();
	dlclose(library_handler);
	return 0;
}
