#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>
#include "sort.h"

static const Options *g_opts = NULL;

static time_t get_entry_time(const Entry *e, TimeType type) {
    if (type == TIME_ATIME) return e->st.st_atime;
    if (type == TIME_CTIME) return e->st.st_ctime;
    return e->st.st_mtime;
}

static int compare_entries(const void *a, const void *b) {
    const Entry *ea = *(const Entry **)a;
    const Entry *eb = *(const Entry **)b;
    int res = 0;

    // Sup xep theo dung luong (-S)
    if (g_opts->sort_size) {
        if (eb->st.st_size > ea->st.st_size) res = 1;
        else if (eb->st.st_size < ea->st.st_size) res = -1;
    } 
    // Sap xep theo thoi gian (-t)
    else if (g_opts->sort_time) {
        time_t ta = get_entry_time(ea, g_opts->time_type);
        time_t tb = get_entry_time(eb, g_opts->time_type);
        if (tb > ta) res = 1;
        else if (tb < ta) res = -1;
    }

    // Neu cung gia tri hoac sap xep mac dinh: so sanh theo ten
    if (res == 0) {
        res = strcmp(ea->name, eb->name);
    }

    // Dao nguoc ket qua neu co co -r
    return g_opts->reverse_sort ? -res : res;
}

void sort_entries(EntryList *list, const Options *opts) {
    if (list == NULL || list->count <= 1) return;
    
    // Khong sap xep neu co co -f
    if (opts->unsorted) return;

    g_opts = opts;
    qsort(list->entries, list->count, sizeof(Entry *), compare_entries);
}
