#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "list.h"
#include "utils.h"

// Khoi tao danh sach
void list_init(EntryList *list) {
    list->entries = NULL;
    list->count = 0;
    list->capacity = 0;
}

// Them Entry vao danh sach
void list_add(EntryList *list, Entry *entry) {
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity == 0
                            ? 16 : list->capacity * 2;
        list->entries = xrealloc(
            list->entries, new_capacity * sizeof(Entry *)
        );
        list->capacity = new_capacity;
    }

    list->entries[list->count++] = entry;
}

// Giai phong danh sach
void list_free(EntryList *list) {
    if (list == NULL) {
        return;
    }

    for (size_t i = 0; i < list->count; i++) {
        entry_free(list->entries[i]);
    }

    free(list->entries);
    list->entries = NULL;
    list->count = 0;
    list->capacity = 0;
}

// Doc noi dung thu muc
void list_read_dir(EntryList *list, const char *dirpath,
                   const Options *opts) {
    DIR *dir = opendir(dirpath);
    if (dir == NULL) {
        perror(dirpath);
        return;
    }

    struct dirent *dp;

    while ((dp = readdir(dir)) != NULL) {
        const char *name = dp->d_name;

        // Loc file an theo -a va -A
        if (opts->show_all == 0) {
            if (opts->show_almost_all) {
                if (strcmp(name, ".") == 0 ||
                    strcmp(name, "..") == 0) {
                    continue;
                }
            } else if (name[0] == '.') {
                continue;
            }
        }

        Entry *entry = entry_create(dirpath, name);
        list_add(list, entry);
    }

    closedir(dir);
}

