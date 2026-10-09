#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "options.h"

// Khoi tao cac tuy chon mac dinh
void init_options(Options *opts) {
    opts->show_almost_all = 0;
    opts->show_all = 0;
    opts->dir_as_file = 0;
    opts->classify = 0;
    opts->unsorted = 0;
    opts->show_inode = 0;
    opts->long_format = 0;
    opts->numeric_uid_gid = 0;
    opts->recursive = 0;
    opts->reverse_sort = 0;
    opts->sort_size = 0;
    opts->show_blocks = 0;
    opts->sort_time = 0;

    opts->time_type = TIME_MTIME;
    opts->size_mode = SIZE_DEFAULT;

    // Mac dinh hien thi ky tu theo trang thai stdout
    opts->char_mode = isatty(STDOUT_FILENO)
                    ? CHAR_PRINTABLE : CHAR_RAW;

    // Bat -A mac dinh neu chay bang root
    if (geteuid() == 0) {
        opts->show_almost_all = 1;
    }
}

// Doc va xu ly cac tuy chon dong lenh
int parse_options(int argc, char *argv[], Options *opts) {
    int opt;
    init_options(opts);

    while ((opt = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
        switch (opt) {
            case 'A': opts->show_almost_all = 1; break;
            case 'a': opts->show_all = 1; break;
            case 'c': opts->time_type = TIME_CTIME; break;
            case 'u': opts->time_type = TIME_ATIME; break;
            case 'd': opts->dir_as_file = 1; break;
            case 'R': opts->recursive = 1; break;
            case 'F': opts->classify = 1; break;

            case 'f':
                opts->unsorted = 1;
                opts->show_all = 1;
                break;

            case 'h': opts->size_mode = SIZE_HUMAN; break;
            case 'i': opts->show_inode = 1; break;
            case 'k': opts->size_mode = SIZE_KIB; break;
            case 'l': opts->long_format = 1; break;

            case 'n':
                opts->long_format = 1;
                opts->numeric_uid_gid = 1;
                break;

            case 'q': opts->char_mode = CHAR_PRINTABLE; break;
            case 'w': opts->char_mode = CHAR_RAW; break;
            case 'r': opts->reverse_sort = 1; break;
            case 'S': opts->sort_size = 1; break;
            case 's': opts->show_blocks = 1; break;
            case 't': opts->sort_time = 1; break;

            default:
                return -1;
        }
    }

    // Khong de -R de quy khi co -d
    if (opts->dir_as_file) {
        opts->recursive = 0;
    }

    return optind;
}

