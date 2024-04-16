#define _GNU_SOURCE

#include <pthread.h>

#include "cache.h"

cache_t *cache_init(int max_count) {
    cache_t *c = malloc(sizeof(cache_t));
    if (!c) {
        printf("Cannot allocate memory for a cache\n");
        abort();
    }

    c->rwlock = malloc(sizeof(pthread_rwlock_t));
    if (!c->rwlock) {
        printf("Cannot allocate memory for a cache\n");
        abort();
    }
    pthread_rwlock_init(c->rwlock, NULL);

    c->first = NULL;
    c->last = NULL;
    c->max_count = max_count;
    c->count = 0;

    return c;
}

cnode_t *cache_add(cache_t *c, char *header) {
    pthread_rwlock_wrlock(c->rwlock);

    cnode_t *new = malloc(sizeof(cnode_t));

    new->header = (char *) calloc(1000, sizeof(char));
    new->answer = (char *) calloc(2000, sizeof(char));
    strcpy(new->header, header);
    new->curr_len = 0;
    new->max_len = 2000;
    new->next = NULL;

    if (c->count == c->max_count) {
        cnode_t *tmp = c->first;
        c->first = tmp->next;

        free(tmp->answer);
        free(tmp->header);
        free(tmp);

        c->count--;
    }
    if (!c->first) {
        c->first = c->last = new;
    } else {
        c->last->next = new;
        c->last = new;
    }
    c->count++;

    pthread_rwlock_unlock(c->rwlock);
    return new;
}

void node_fill(cache_t *c, cnode_t *cnode, char *message) {
    pthread_rwlock_wrlock(c->rwlock);
    while (cnode->max_len < cnode->curr_len + strlen(message)) {
        cnode->answer = (char *) realloc(cnode->answer, cnode->max_len * 2);
    }
    strcpy(&cnode->answer[cnode->curr_len], message);
    cnode->curr_len += strlen(message);
    cnode->max_len *= 2;
    pthread_rwlock_unlock(c->rwlock);
}

cnode_t *cache_find(cache_t *c, char *header) {
    pthread_rwlock_rdlock(c->rwlock);
    cnode_t *cnode = c->first;
    while (cnode != NULL) {
        if (!strcmp(cnode->header, header)) {
            pthread_rwlock_unlock(c->rwlock);
            return cnode;
        }
        cnode = cnode->next;
    }
    pthread_rwlock_unlock(c->rwlock);
    return NULL;
}

int get_message(cache_t *c, cnode_t *cnode, char *message, int *it) {
    pthread_rwlock_rdlock(c->rwlock);
    if (*it == cnode->curr_len && cnode->isFilled) {
        return -1;
    }
    strcpy(message + (*it), cnode->answer + (*it));
    int len = strlen(cnode->answer + (*it));
    *it += len;
    pthread_rwlock_unlock(c->rwlock);
    return len;
}

void set_filled(cnode_t *cnode, int flag) {
    cnode->isFilled = flag;
}
