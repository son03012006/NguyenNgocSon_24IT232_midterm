#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

// Cap phat bo nho an toan
void *xmalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL && size > 0) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

// Cap phat lai bo nho an toan
void *xrealloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL && size > 0) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

// Sao chep chuoi va cap phat bo nho
char *xstrdup(const char *s) {
    if (s == NULL) {
        return NULL;
    }

    size_t len = strlen(s) + 1;
    char *dup = xmalloc(len);
    memcpy(dup, s, len);
    return dup;
}


