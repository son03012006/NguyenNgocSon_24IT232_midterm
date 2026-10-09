#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "options.h"
#include "entry.h"
#include "list.h"
#include "sort.h"
#include "print.h"
#include "utils.h"

// Khai bao ham de quy khi su dung tuy chon -R
static void recursive_list(const char *dirpath, const Options *opts, int is_sub);

// So sanh ten cac operand de sap xep theo thu tu chu cai
static int compare_operand_names(const void *a, const void *b) {
    const Entry *ea = *(const Entry **)a;
    const Entry *eb = *(const Entry **)b;
    return strcmp(ea->name, eb->name);
}

// Liet ke cac file va thu muc trong mot thu muc cu the
static int process_directory(const char *dirpath, const Options *opts, int show_header) {
    // In ten thu muc truoc danh sach neu can
    if (show_header) {
        printf("\n%s:\n", dirpath);
    }

    EntryList list;
    list_init(&list);

    // Doc danh sach cac muc trong thu muc
    if (list_read_dir(&list, dirpath, opts) != 0) {
        list_free(&list);
        return -1;
    }

    sort_entries(&list, opts);
    print_entries(&list, opts);

    // Neu bat -R thi thu thap va duyet cac thu muc con
    if (opts->recursive) {
        // Luu duong dan thu muc con truoc khi giai phong danh sach
        size_t sub_count = 0;
        char **sub_paths = NULL;

        for (size_t i = 0; i < list.count; i++) {
            Entry *e = list.entries[i];
            if (e->error == 0 && S_ISDIR(e->st.st_mode)) {
                // Bo qua hai muc dac biet . va ..
                if (strcmp(e->name, ".") != 0 && strcmp(e->name, "..") != 0) {
                    sub_paths = realloc(sub_paths, (sub_count + 1) * sizeof(char *));
                    sub_paths[sub_count++] = xstrdup(e->path);
                }
            }
        }

        list_free(&list);

        // Goi de quy cho tung thu muc con
        for (size_t i = 0; i < sub_count; i++) {
            recursive_list(sub_paths[i], opts, 1);
            free(sub_paths[i]);
        }
        free(sub_paths);
    } else {
        list_free(&list);
    }

    return 0;
}

// Ham goi de quy de liet ke mot thu muc
static void recursive_list(const char *dirpath, const Options *opts, int is_sub) {
    (void)is_sub;
    process_directory(dirpath, opts, 1);
}

// Ham chinh xu ly cac tuy chon va doi so dong lenh
int main(int argc, char *argv[]) {
    Options opts;
    int arg_idx = parse_options(argc, argv, &opts);

    // Hien thi huong dan neu tuy chon khong hop le
    if (arg_idx == -1) {
        fprintf(stderr, "usage: ls [-AacdFfhiklnqRrSstuw] [file ...]\n");
        return EXIT_FAILURE;
    }

    // Neu khong co operand thi mac dinh liet ke thu muc hien tai
    if (arg_idx >= argc) {
        EntryList list;
        list_init(&list);
        if (list_read_dir(&list, ".", &opts) != 0) {
            list_free(&list);
            return EXIT_FAILURE;
        }
        sort_entries(&list, &opts);
        print_entries(&list, &opts);
        list_free(&list);
        return EXIT_SUCCESS;
    }

    int num_operands = argc - arg_idx;

    // Xu ly truong hop chi co mot operand
    if (num_operands == 1) {
        const char *path = argv[arg_idx];
        struct stat st;
        int stat_res;

        // Chon lstat hoac stat tuy theo tuy chon -d
        if (opts.dir_as_file) {
            stat_res = lstat(path, &st);
        } else {
            stat_res = stat(path, &st);
        }

        // Bao loi neu khong lay duoc thong tin operand
        if (stat_res == -1) {
            fprintf(stderr, "ls: %s: %s\n", path, strerror(errno));
            return EXIT_FAILURE;
        }

        // Neu la file hoac co -d thi in operand nhu mot muc
        if (!S_ISDIR(st.st_mode) || opts.dir_as_file) {
            EntryList list;
            list_init(&list);
            Entry *entry = entry_create("", path);
            list_add(&list, entry);
            sort_entries(&list, &opts);
            print_entries(&list, &opts);
            list_free(&list);
            return EXIT_SUCCESS;
        } else {
            // Neu la thu muc thi liet ke cac muc ben trong
            if (process_directory(path, &opts, 0) != 0) {
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        }
    }

    // Chia nhieu operand thanh hai nhom file va thu muc
    EntryList file_list;
    EntryList dir_list;
    list_init(&file_list);
    list_init(&dir_list);

    int exit_status = EXIT_SUCCESS;

    for (int i = arg_idx; i < argc; i++) {
        const char *path = argv[i];
        struct stat st;
        int stat_res;

        // Lay thong tin operand, giu nguyen symbolic link neu co -d
        if (opts.dir_as_file) {
            stat_res = lstat(path, &st);
        } else {
            stat_res = stat(path, &st);
        }

        // Ghi nhan loi va tiep tuc xu ly cac operand con lai
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

    // Sap xep rieng hai nhom operand theo ten
    if (file_list.count > 1) {
        qsort(file_list.entries, file_list.count, sizeof(Entry *), compare_operand_names);
    }
    if (dir_list.count > 1) {
        qsort(dir_list.entries, dir_list.count, sizeof(Entry *), compare_operand_names);
    }

    // In nhom file truoc nhom thu muc
    if (file_list.count > 0) {
        print_entries(&file_list, &opts);
    }

    // Them dong trong neu co ca file va thu muc
    if (file_list.count > 0 && dir_list.count > 0) {
        printf("\n");
    }

    // Duyet va in tung thu muc trong nhom thu muc
    for (size_t i = 0; i < dir_list.count; i++) {
        if (file_list.count > 0 || dir_list.count > 1 || opts.recursive) {
            if (i > 0 || file_list.count > 0) {
                printf("\n");
            }
            printf("%s:\n", dir_list.entries[i]->path);
        }
        
        EntryList single_dir_list;
        list_init(&single_dir_list);

        // Doc va in noi dung cua thu muc
        if (list_read_dir(&single_dir_list, dir_list.entries[i]->path, &opts) == 0) {
            sort_entries(&single_dir_list, &opts);
            print_entries(&single_dir_list, &opts);

            // Neu co -R thi thu thap cac thu muc con de de quy
            if (opts.recursive) {
                size_t sub_c = 0;
                char **sub_p = NULL;

                for (size_t j = 0; j < single_dir_list.count; j++) {
                    Entry *sub_e = single_dir_list.entries[j];
                    if (sub_e->error == 0 && S_ISDIR(sub_e->st.st_mode)) {
                        // Bo qua . va .. de tranh lap vo han
                        if (strcmp(sub_e->name, ".") != 0 && strcmp(sub_e->name, "..") != 0) {
                            sub_p = realloc(sub_p, (sub_c + 1) * sizeof(char *));
                            sub_p[sub_c++] = xstrdup(sub_e->path);
                        }
                    }
                }

                single_dir_list.count = single_dir_list.count; // Khong thay doi so luong entry
                single_dir_list.entries = single_dir_list.entries; // Giu nguyen danh sach entry

                // Goi de quy cho cac thu muc con
                for (size_t j = 0; j < sub_c; j++) {
                    recursive_list(sub_p[j], &opts, 1);
                    free(sub_p[j]);
                }
                free(sub_p);
            }
            list_free(&single_dir_list);
        }
    }

    // Giai phong bo nho cua hai danh sach operand
    list_free(&file_list);
    list_free(&dir_list);

    return exit_status;
}

