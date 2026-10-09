#include <unistd.h>
#include <stdio.h>
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
    opts->char_mode = isatty(STDOUT_FILENO)
                    ? CHAR_PRINTABLE : CHAR_RAW;
}

// Phan tich cac tuy chon dong lenh
int parse_options(int argc, char *argv[], Options *opts) {
    int opt;
    init_options(opts);

    while ((opt = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
        switch (opt) {
            case 'A': opts->show_almost_all = 1; break;
            case 'a': opts->show_all = 1; break;
            case 'c': opts->time_type = TIME_CTIME; break;
            case 'u': opts->time_type = TIME_ATIME; break;
            case 'd':
                opts->dir_as_file = 1;
                opts->recursive = 0;
                break;
            case 'R':
                opts->recursive = 1;
                opts->dir_as_file = 0;
                break;
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
            default: return -1;
        }
    }

    return optind;
}

