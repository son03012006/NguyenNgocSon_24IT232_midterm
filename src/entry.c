#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "entry.h"
#include "utils.h"

// Tao Entry va lay thong tin bang lstat()
Entry *entry_create(const char *dirpath, const char *name) {
    Entry *e = xmalloc(sizeof(Entry));
    e->name = xstrdup(name);
    e->error = 0;

    if (dirpath && dirpath[0] != '\0' &&
        strcmp(dirpath, ".") != 0) {
        size_t len = strlen(dirpath) + strlen(name) + 2;
        e->path = xmalloc(len);
        snprintf(e->path, len, "%s/%s", dirpath, name);
    } else {
        e->path = xstrdup(name);
    }

    // Dung lstat de giu nguyen thong tin symlink
    if (lstat(e->path, &e->st) == -1) {
        e->error = 1;
    }

    return e;
}

// Giai phong Entry
void entry_free(Entry *e) {
    if (e == NULL) {
        return;
    }

    free(e->name);
    free(e->path);
    free(e);
}


