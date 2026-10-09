#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

// Cap phat bo nho an toan
void *xmalloc(size_t size);

// Cap phat lai bo nho an toan
void *xrealloc(void *ptr, size_t size);

// Sao chep chuoi va cap phat bo nho
char *xstrdup(const char *s);

#endif
