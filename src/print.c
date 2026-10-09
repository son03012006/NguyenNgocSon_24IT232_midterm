#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include "print.h"
#include "format.h"

// In danh sach Entry
void print_entries(const EntryList *list, const Options *opts) {
    char mode_str[11];
    char size_str[32];
    char time_str[32];

    for (size_t i = 0; i < list->count; i++) {
        Entry *e = list->entries[i];

        // Bao loi neu khong lay duoc thong tin file
        if (e->error != 0) {
            fprintf(stderr, "ls: %s: %s\n",
                    e->path, strerror(e->error));
            continue;
        }

        // Hien thi inode (-i)
        if (opts->show_inode) {
            printf("%llu ", (unsigned long long)e->st.st_ino);
        }

        // Hien thi so block (-s)
        if (opts->show_blocks) {
            printf("%lld ",
                   (long long)((e->st.st_blocks + 1) / 2));
        }

        // In thong tin chi tiet (-l hoac -n)
        if (opts->long_format) {
            format_mode(e->st.st_mode, mode_str);
            format_size(e, opts, size_str, sizeof(size_str));
            format_time(e, opts, time_str, sizeof(time_str));

            struct passwd *pw = getpwuid(e->st.st_uid);
            struct group *gr = getgrgid(e->st.st_gid);

            printf("%s %2u ",
                   mode_str, (unsigned int)e->st.st_nlink);

            if (opts->numeric_uid_gid || pw == NULL)
                printf("%u ", (unsigned int)e->st.st_uid);
            else
                printf("%s ", pw->pw_name);

            if (opts->numeric_uid_gid || gr == NULL)
                printf("%u ", (unsigned int)e->st.st_gid);
            else
                printf("%s ", gr->gr_name);

            printf("%8s %s ", size_str, time_str);
        }

        // In ten file va ky tu phan loai (-F)
        printf("%s", e->name);

        if (opts->classify) {
            if (S_ISDIR(e->st.st_mode)) printf("/");
            else if (S_ISLNK(e->st.st_mode)) printf("@");
            else if (S_ISFIFO(e->st.st_mode)) printf("|");
            else if (S_ISSOCK(e->st.st_mode)) printf("=");
            else if (e->st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
                printf("*");
        }

        printf("\n");
    }
}
