#define _GNU_SOURCE

#include <pthread.h>

#include "cache.h"

cache_t *cache_init(int max_count) {
    cache_t *c = malloc(sizeof(cache_t));
    if (!c) {
        printf("Cannot allocate memory for a cache\n");
        abort();
    }

    c->add_sync = malloc(sizeof(pthread_rwlock_t));
    if (!c->add_sync) {
        printf("Cannot allocate memory for a cache\n");
        abort();
    }
    pthread_rwlock_init(c->add_sync, NULL);

    c->first = NULL;
    c->last = NULL;
    c->max_count = max_count;
    c->count = 0;

    return c;
}

cnode_t *cache_add(cache_t *c, char *header) {
    pthread_rwlock_wrlock(c->add_sync);

    cnode_t *new = malloc(sizeof(cnode_t));

    new->fill_sync = malloc(sizeof(pthread_rwlock_t));
    if (!new->fill_sync) {
        printf("Cannot allocate memory for a cache\n");
        abort();
    }
    pthread_rwlock_init(new->fill_sync, NULL);

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

    pthread_rwlock_unlock(c->add_sync);
    return new;
}

void node_fill(cnode_t *cnode, char *message, int len) {
    pthread_rwlock_wrlock(cnode->fill_sync);

    while (cnode->max_len < cnode->curr_len + len + 1) {
        cnode->max_len *= 2;
        cnode->answer = (char *) realloc(cnode->answer, cnode->max_len);
    }

    for (int i = 0; i < len; i++) {
        cnode->answer[cnode->curr_len++] = message[i];
    }

    pthread_rwlock_unlock(cnode->fill_sync);
}

cnode_t *cache_find(cache_t *c, char *header) {
    pthread_rwlock_rdlock(c->add_sync);
    cnode_t *cnode = c->first;
    while (cnode != NULL) {
        if (!strcmp(cnode->header, header)) {
            pthread_rwlock_unlock(c->add_sync);
            return cnode;
        }
        cnode = cnode->next;
    }
    pthread_rwlock_unlock(c->add_sync);
    return NULL;
}

int get_message(cnode_t *cnode, char *buffer, int *it, int max_size) {
    pthread_rwlock_rdlock(cnode->fill_sync);

    if (*it == cnode->curr_len && cnode->isFilled) {
        pthread_rwlock_unlock(cnode->fill_sync);
        return -1;
    }

    int len;
    for (len = 0; *it + len < cnode->curr_len && len < max_size; len++) {
        buffer[len] = cnode->answer[*it + len];
    }
    *it += len;

    pthread_rwlock_unlock(cnode->fill_sync);

    return len;
}

void set_filled(cnode_t *cnode, int flag) {
    pthread_rwlock_rdlock(cnode->fill_sync);

    cnode->isFilled = flag;

    pthread_rwlock_unlock(cnode->fill_sync);
}
