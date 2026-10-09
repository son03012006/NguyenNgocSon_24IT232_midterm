#ifndef SORT_H
#define SORT_H

#include "list.h"
#include "options.h"

// Sap xep danh sach file theo cac co -t, -S, -r, -c, -u
void sort_entries(EntryList *list, const Options *opts);

#endif
