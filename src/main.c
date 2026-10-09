#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "options.h"
#include "list.h"
#include "sort.h"
#include "print.h"

int main(int argc, char *argv[]) {
    Options opts;
    int arg_idx = parse_options(argc, argv, &opts);

    // Bao loi neu tuy chon khong hop le
    if (arg_idx == -1) {
        fprintf(stderr,
                "usage: ls [-AacdFfhiklnqRrSstuw] [file ...]\n");
        return EXIT_FAILURE;
    }

    // Mac dinh hien thi thu muc hien tai
    const char *path = arg_idx < argc ? argv[arg_idx] : ".";

    struct stat st;

    // Lay thong tin file hoac thu muc
    if (stat(path, &st) == -1) {
        perror(path);
        return EXIT_FAILURE;
    }

    // Neu la file hoac co -d thi xu ly nhu mot muc don
    if (!S_ISDIR(st.st_mode) || opts.dir_as_file) {
        EntryList list;
        list_init(&list);

        Entry *entry = entry_create(".", path);
        list_add(&list, entry);

        sort_entries(&list, &opts);
        print_entries(&list, &opts);

        list_free(&list);
        return EXIT_SUCCESS;
    }

    // Doc cac muc trong thu muc
    EntryList list;
    list_init(&list);

    if (list_read_dir(&list, path, &opts) != 0) {
        list_free(&list);
        return EXIT_FAILURE;
    }

    // Sap xep va in danh sach
    sort_entries(&list, &opts);
    print_entries(&list, &opts);

    list_free(&list);
    return EXIT_SUCCESS;
}
