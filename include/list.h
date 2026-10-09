#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include "entry.h"
#include "options.h"

// Struct danh sach quan ly mang con tro Entry dong
typedef struct {
    Entry **entries;
    size_t count;
    size_t capacity;
} EntryList;

void list_init(EntryList *list);
void list_add(EntryList *list, Entry *entry);
void list_free(EntryList *list);

// Doi sang tra ve int: 0 neu thanh cong, -1 neu opendir that bai
int list_read_dir(EntryList *list, const char *dirpath, const Options *opts);

#endif
