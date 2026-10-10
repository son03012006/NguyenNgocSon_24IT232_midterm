#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <locale.h>
#include <sys/stat.h>
#include "options.h"
#include "entry.h"
#include "list.h"
#include "sort.h"
#include "print.h"
#include "utils.h"

// Xu ly mot thu muc (bao gom de quy). Tra ve 0 neu thanh cong, -1 neu co loi
static int process_directory(const char *dirpath, const Options *opts,
                             int show_header, int blank_before) {
    int status = 0;

    // Chi in dong trong truoc header khi truoc do da co output
    if (show_header) {
        if (blank_before) printf("\n");
        printf("%s:\n", dirpath);
    }

    EntryList list;
    list_init(&list);

    if (list_read_dir(&list, dirpath, opts) != 0) {
        list_free(&list);
        return -1;
    }

    sort_entries(&list, opts);
    print_entries(&list, opts, 1); // thu muc: in total

    if (opts->recursive) {
        size_t sub_count = 0;
        char **sub_paths = NULL;

        for (size_t i = 0; i < list.count; i++) {
            Entry *e = list.entries[i];
            if (e->error == 0 && S_ISDIR(e->st.st_mode) &&
                strcmp(e->name, ".") != 0 && strcmp(e->name, "..") != 0) {
                sub_paths = xrealloc(sub_paths, (sub_count + 1) * sizeof(char *));
                sub_paths[sub_count++] = xstrdup(e->path);
            }
        }

        list_free(&list); // Giai phong truoc khi de quy

        for (size_t i = 0; i < sub_count; i++) {
            if (process_directory(sub_paths[i], opts, 1, 1) != 0) {
                status = -1; // Ghi nhan loi o thu muc con
            }
            free(sub_paths[i]);
        }
        free(sub_paths);
    } else {
        list_free(&list);
    }

    return status;
}

int main(int argc, char *argv[]) {
    setlocale(LC_CTYPE, "");
    Options opts;
    int arg_idx = parse_options(argc, argv, &opts);

    if (arg_idx == -1) {
        fprintf(stderr, "usage: my_ls [-AacdFfhiklnqRrSstuw] [file ...]\n");
        return EXIT_FAILURE;
    }

    // Khong co operand thi coi nhu "."
    char dot[] = ".";
    char *default_args[] = { dot };
    char **operands = argv + arg_idx;
    int num_operands = argc - arg_idx;

    if (num_operands == 0) {
        operands = default_args;
        num_operands = 1;
    }

    // Khai bao truoc vong lap de ca ham deu dung duoc
    EntryList file_list, dir_list;
    list_init(&file_list);
    list_init(&dir_list);
    int exit_status = EXIT_SUCCESS;

    // Phan loai operand thanh file va thu muc
    for (int i = 0; i < num_operands; i++) {
        const char *path = operands[i];
        struct stat st;

        // Chi di theo symlink o operand khi khong co -d, -l, -F
        int follow = !(opts.dir_as_file || opts.long_format || opts.classify);
        int stat_res = follow ? stat(path, &st) : lstat(path, &st);

        // Symlink hong: in chinh symlink thay vi bao loi
        if (stat_res == -1 && follow) {
            stat_res = lstat(path, &st);
        }

        if (stat_res == -1) {
            fprintf(stderr, "ls: %s: %s\n", path, strerror(errno));
            exit_status = EXIT_FAILURE;
            continue;
        }

        Entry *e = entry_create("", path);
        if (!S_ISDIR(st.st_mode) || opts.dir_as_file) {
            list_add(&file_list, e);
        } else {
            list_add(&dir_list, e);
        }
    }

    // Sap xep rieng hai nhom theo cac co -t, -S, -r, -f
    sort_entries(&file_list, &opts);
    sort_entries(&dir_list, &opts);

    // In nhom file truoc (khong in total)
    if (file_list.count > 0) {
        print_entries(&file_list, &opts, 0);
    }

    // In nhom thu muc
    int show_header = (file_list.count > 0 || dir_list.count > 1 || opts.recursive);
    for (size_t i = 0; i < dir_list.count; i++) {
        int blank_before = (i > 0 || file_list.count > 0);
        if (process_directory(dir_list.entries[i]->path, &opts,
                              show_header, blank_before) != 0) {
            exit_status = EXIT_FAILURE;
        }
    }

    list_free(&file_list);
    list_free(&dir_list);
    return exit_status;
}
