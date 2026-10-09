#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include "entry.h"
#include "options.h"

// Danh sach cac file
typedef struct {
    Entry **entries;
    size_t count;
    size_t capacity;
} EntryList;

// Khoi tao danh sach
void list_init(EntryList *list);

// Them file vao danh sach
void list_add(EntryList *list, Entry *entry);

// Giai phong danh sach
void list_free(EntryList *list);

// Doc noi dung thu muc
void list_read_dir(EntryList *list, const char *dirpath,
                   const Options *opts);

#endif
