#ifndef ENTRY_H
#define ENTRY_H

#include <sys/stat.h>
#include <sys/types.h>

// Struct luu thong tin file/thu muc dung con tro chuoi dong (tranh tran bo nho)
typedef struct {
    char *name;      // Ten file
    char *path;      // Duong dan file day du
    struct stat st;  // Thong tin file tu lstat()
    int error;       // Bao loi neu lstat that bai
} Entry;

// Ham khoi tao va giai phong bo nho dong
Entry *entry_create(const char *dirpath, const char *name);
void entry_free(Entry *e);

#endif

