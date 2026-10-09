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

    opts->time_type = TIME_MTIME;       // Mac dinh su dung thoi gian sua doi
    opts->size_mode = SIZE_DEFAULT;     // Mac dinh hien thi kich thuoc file
    opts->char_mode = isatty(STDOUT_FILENO) ? CHAR_PRINTABLE : CHAR_RAW;

    // Neu chay voi quyen root thi bat tuy chon -A
    if (geteuid() == 0) {
        opts->show_almost_all = 1;
    }
}

// Phan tich cac tuy chon tu dong lenh
int parse_options(int argc, char *argv[], Options *opts) {
    int opt;
    init_options(opts);

    // Doc va xu ly tung tuy chon duoc truyen vao
    while ((opt = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
        switch (opt) {
            case 'A': opts->show_almost_all = 1; break; // Hien thi cac muc an, tru . va ..
            case 'a': opts->show_all = 1; break;        // Hien thi tat ca file, ke ca file an
            
            // Chon loai thoi gian hien thi
            case 'c': opts->time_type = TIME_CTIME; break;
            case 'u': opts->time_type = TIME_ATIME; break;
            
            // -d va -R ghi de nhau theo thu tu xuat hien
            case 'd':
                opts->dir_as_file = 1;
                opts->recursive = 0;
                break;
            case 'R':
                opts->recursive = 1;
                opts->dir_as_file = 0;
                break;
                
            case 'F': opts->classify = 1; break; // Them ky tu phan biet loai file
            case 'f':
                opts->unsorted = 1;              // Khong sap xep danh sach
                opts->show_all = 1;              // Hien thi ca file an
                break;
            case 'h': opts->size_mode = SIZE_HUMAN; break; // Kich thuoc de doc
            case 'i': opts->show_inode = 1; break;         // Hien thi so inode
            case 'k': opts->size_mode = SIZE_KIB; break;   // Kich thuoc theo KiB
            
            // -l va -n ghi de nhau theo thu tu xuat hien
            case 'l':
                opts->long_format = 1;
                opts->numeric_uid_gid = 0;
                break;
            case 'n':
                opts->long_format = 1;
                opts->numeric_uid_gid = 1;
                break;
                
            // Chon cach hien thi ky tu khong in duoc
            case 'q': opts->char_mode = CHAR_PRINTABLE; break;
            case 'w': opts->char_mode = CHAR_RAW; break;
            
            case 'r': opts->reverse_sort = 1; break; // Dao nguoc thu tu sap xep
            case 'S': opts->sort_size = 1; break;    // Sap xep theo kich thuoc
            case 's': opts->show_blocks = 1; break;  // Hien thi so block
            case 't': opts->sort_time = 1; break;    // Sap xep theo thoi gian

            default: return -1; // Bao loi neu gap tuy chon khong hop le
        }
    }

    return optind; // Vi tri doi so dau tien khong phai tuy chon
}


