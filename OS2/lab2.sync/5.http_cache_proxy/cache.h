#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct CacheNode {
    char *header;
    char *answer;
    int max_len;
    int curr_len;
    int isFilled;
    struct CacheNode *next;
} cnode_t;

typedef struct Cache {
    cnode_t *first;
    cnode_t *last;

    pthread_rwlock_t *rwlock;

    int count;
    int max_count;
} cache_t;

cache_t *cache_init(int max_count);

cnode_t *cache_add(cache_t *c, char *header);

void node_fill(cache_t *c, cnode_t *cnode, char *message);

cnode_t *cache_find(cache_t *c, char *header);

int get_message(cache_t *c, cnode_t *cnode, char *message, int *it);

void set_filled(cnode_t *cnode, int flag);