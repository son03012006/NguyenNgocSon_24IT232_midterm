#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "entry.h"
#include "utils.h"

Entry *entry_create(const char *dirpath, const char *name) {
    Entry *e = xmalloc(sizeof(Entry));
    e->name = xstrdup(name);
    e->error = 0;

    if (dirpath && dirpath[0] != '\0' &&
        strcmp(dirpath, ".") != 0) {
        size_t len = strlen(dirpath) + strlen(name) + 2;
        e->path = xmalloc(len);

        // Ghep duong dan, tranh lap dau /
        size_t dir_len = strlen(dirpath);
        if (dirpath[dir_len - 1] == '/') {
            snprintf(e->path, len, "%s%s", dirpath, name);
        } else {
            snprintf(e->path, len, "%s/%s", dirpath, name);
        }
    } else {
        e->path = xstrdup(name);
    }

    // Luu ma loi neu khong doc duoc thong tin file
    if (lstat(e->path, &e->st) == -1) {
        e->error = errno;
    }

    return e;
}

void entry_free(Entry *e) {
    if (e == NULL) return;

    free(e->name);
    free(e->path);
    free(e);
}
