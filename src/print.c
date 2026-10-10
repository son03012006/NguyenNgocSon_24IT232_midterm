#define _NETBSD_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <pwd.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#include <grp.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "print.h"
#include "format.h"

// In ten file va thay ky tu khong in duoc bang dau ?
static void print_filename(const char *name, const Options *opts) {
    if (opts->char_mode != CHAR_PRINTABLE) {
        printf("%s", name);
        return;
    }

    mbstate_t state;
    memset(&state, 0, sizeof(state));

    const char *p = name;
    size_t remaining = strlen(name);

    while (remaining > 0) {
        wchar_t wc;
        size_t len = mbrtowc(&wc, p, remaining, &state);

        if (len == (size_t)-1 || len == (size_t)-2) {
            putchar('?');
            p++;
            remaining--;
            memset(&state, 0, sizeof(state));
        } else if (len == 0) {
            break;
        } else {
            putwchar(iswprint(wc) ? wc : L'?');
            p += len;
            remaining -= len;
        }
    }
}

// In dong total cho mot danh sach
static void print_total_line(const EntryList *list, const Options *opts) {
    char total_str[32];
    long long total_blocks = 0;

    for (size_t i = 0; i < list->count; i++) {
        if (list->entries[i]->error == 0) {
            total_blocks += get_entry_blocks(list->entries[i], opts);
        }
    }

    if (opts->size_mode == SIZE_HUMAN) {
        format_human(total_blocks * 512, total_str, sizeof(total_str));
    } else {
        snprintf(total_str, sizeof(total_str), "%lld", total_blocks);
    }

    printf("total %s\n", total_str);
}

// In danh sach file theo cac tuy chon
void print_entries(const EntryList *list, const Options *opts, int print_total) {
    char mode_str[11];
    char size_str[32];
    char time_str[32];
    char blk_str[32];

    // Chi in total cho -s khi dau ra la terminal; -l thi luon in
    int is_term = isatty(STDOUT_FILENO);
    if (print_total && (opts->long_format || (opts->show_blocks && is_term))) {
        print_total_line(list, opts);
    }

    // Duyet va in tung entry trong danh sach
    for (size_t i = 0; i < list->count; i++) {
        Entry *e = list->entries[i];

        // Bo qua entry bi loi va hien thi thong bao
        if (e->error != 0) {
            fprintf(stderr, "ls: %s: %s\n", e->path, strerror(e->error));
            continue;
        }

        // Hien thi inode khi co -i
        if (opts->show_inode) {
            printf("%llu ", (unsigned long long)e->st.st_ino);
        }

        // Hien thi so block (hoac kich thuoc de doc voi -h) khi co -s
        if (opts->show_blocks) {
            format_block_count(e, opts, blk_str, sizeof(blk_str));
            printf("%s ", blk_str);
        }

        // In thong tin chi tiet khi co -l hoac -n
        if (opts->long_format) {
            format_mode(e->st.st_mode, mode_str);
            format_time(e, opts, time_str, sizeof(time_str));

            // Lay ten nguoi dung va ten nhom tu UID, GID
            struct passwd *pw = getpwuid(e->st.st_uid);
            struct group *gr = getgrgid(e->st.st_gid);

            printf("%s %2u ", mode_str, (unsigned int)e->st.st_nlink);

            // In UID dang so hoac ten nguoi dung
            if (opts->numeric_uid_gid || pw == NULL) printf("%u ", (unsigned int)e->st.st_uid);
            else printf("%s ", pw->pw_name);

            // In GID dang so hoac ten nhom
            if (opts->numeric_uid_gid || gr == NULL) printf("%u ", (unsigned int)e->st.st_gid);
            else printf("%s ", gr->gr_name);

            // File thiet bi hien thi major, minor thay cho kich thuoc
            if (S_ISCHR(e->st.st_mode) || S_ISBLK(e->st.st_mode)) {
                printf("%3d, %3d ", (int)major(e->st.st_rdev), (int)minor(e->st.st_rdev));
            } else {
                format_size(e, opts, size_str, sizeof(size_str));
                printf("%8s ", size_str);
            }

            printf("%s ", time_str);
        }

        // In ten file theo che do ky tu da chon
        print_filename(e->name, opts);

        // Them ky tu phan loai file khi co -F
        if (opts->classify) {
            if (S_ISDIR(e->st.st_mode)) printf("/");
            else if (S_ISLNK(e->st.st_mode)) printf("@");
            else if (S_ISFIFO(e->st.st_mode)) printf("|");
            else if (S_ISSOCK(e->st.st_mode)) printf("=");
            else if (S_ISWHT(e->st.st_mode)) printf("%%");
            else if (e->st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) printf("*");
        }

        // Hien thi duong dan dich cua symbolic link khi co -l
        if (opts->long_format && S_ISLNK(e->st.st_mode)) {
            char linkbuf[1024];
            ssize_t len = readlink(e->path, linkbuf, sizeof(linkbuf) - 1);
            if (len != -1) {
                linkbuf[len] = '\0';
                printf(" -> %s", linkbuf);
            }
        }

        printf("\n");
    }
}
