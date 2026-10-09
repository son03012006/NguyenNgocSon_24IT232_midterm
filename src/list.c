#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "list.h"
#include "utils.h"

// Khoi tao danh sach
void list_init(EntryList *list) {
    list->entries = NULL;
    list->count = 0;
    list->capacity = 0;
}

// Them Entry vao danh sach, tu tang dung luong neu can
void list_add(EntryList *list, Entry *entry) {
    if (list->count >= list->capacity) {
        size_t new_capacity =
            list->capacity == 0 ? 16 : list->capacity * 2;

        list->entries = xrealloc(
            list->entries, new_capacity * sizeof(Entry *)
        );
        list->capacity = new_capacity;
    }

    list->entries[list->count++] = entry;
}

// Giai phong cac Entry va mang con tro
void list_free(EntryList *list) {
    if (list == NULL) return;

    for (size_t i = 0; i < list->count; i++) {
        entry_free(list->entries[i]);
    }

    free(list->entries);
    list->entries = NULL;
    list->count = 0;
    list->capacity = 0;
}

// Doc cac muc trong thu muc va loc file an
int list_read_dir(EntryList *list, const char *dirpath,
                  const Options *opts) {
    DIR *dir = opendir(dirpath);

    if (dir == NULL) {
        fprintf(stderr, "ls: %s: %s\n", dirpath, strerror(errno));
        return -1;
    }

    struct dirent *dp;
    int read_error = 0;

    // Doc tung muc cho den khi het du lieu hoac gap loi
    while (1) {
        errno = 0;
        dp = readdir(dir);

        if (dp == NULL) {
            if (errno != 0) read_error = errno;
            break;
        }

        const char *name = dp->d_name;

        // -a hien thi tat ca, -A bo qua . va ..
        if (!opts->show_all) {
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

    // Ghi nhan loi dong thu muc neu chua co loi doc
    if (closedir(dir) == -1 && read_error == 0) {
        read_error = errno;
    }

    if (read_error != 0) {
        fprintf(stderr, "ls: %s: %s\n",
                dirpath, strerror(read_error));
        return -1;
    }

    return 0;
}


