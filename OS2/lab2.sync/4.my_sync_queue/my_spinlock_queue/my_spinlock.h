#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdatomic.h>

typedef struct {
	int lock;
} my_spinlock_t;

void my_spinlock_init(my_spinlock_t* s);
void my_spinlock_lock(my_spinlock_t* s);
void my_spinlock_unlock(my_spinlock_t* s);

