#define _NETBSD_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <sys/stat.h>
#include "format.h"

// Chuyen quyen va loai file thanh chuoi 10 ky tu
void format_mode(mode_t mode, char *str) {
    // Xac dinh loai file
    if (S_ISDIR(mode)) str[0] = 'd';
    else if (S_ISLNK(mode)) str[0] = 'l';
    else if (S_ISCHR(mode)) str[0] = 'c';
    else if (S_ISBLK(mode)) str[0] = 'b';
    else if (S_ISFIFO(mode)) str[0] = 'p';
    else if (S_ISSOCK(mode)) str[0] = 's';
#ifdef S_ISWHT
    else if (S_ISWHT(mode)) str[0] = 'w'; 
#endif
    else str[0] = '-';

    // Quyen doc va ghi cua chu so huu
    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';

    // Xu ly quyen SUID va quyen thuc thi cua chu so huu
    if (mode & S_ISUID) str[3] = (mode & S_IXUSR) ? 's' : 'S';
    else str[3] = (mode & S_IXUSR) ? 'x' : '-';

    // Quyen doc va ghi cua nhom
    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';

    // Xu ly quyen SGID
    if (mode & S_ISGID) str[6] = (mode & S_IXGRP) ? 's' : 'S';
    else str[6] = (mode & S_IXGRP) ? 'x' : '-';

    // Quyen doc va ghi cua nguoi dung khac
    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';

    // Xu ly quyen Sticky Bit
    if (mode & S_ISVTX) str[9] = (mode & S_IXOTH) ? 't' : 'T';
    else str[9] = (mode & S_IXOTH) ? 'x' : '-';

    str[10] = '\0'; // Ket thuc chuoi
}

// Dinh dang kich thuoc file theo tuy chon
void format_size(const Entry *e, const Options *opts, char *buf, size_t size) {
    // Lay kich thuoc file theo byte
    double bytes = (double)e->st.st_size;

    if (opts->size_mode == SIZE_HUMAN) {
        if (bytes < 1024) {
            snprintf(buf, size, "%lldB", (long long)bytes);
        } else {
            // Danh sach don vi kich thuoc
            const char *units[] = {"B", "K", "M", "G", "T"};
            int i = 0;

            // Chia cho 1024 de chuyen sang don vi lon hon
            while (bytes >= 1024 && i < 4) {
                bytes /= 1024;
                i++;
            }
            snprintf(buf, size, "%.1f%s", bytes, units[i]);
        }
    } else {
        // Hien thi kich thuoc theo byte
        snprintf(buf, size, "%lld", (long long)e->st.st_size);
    }
}

// Tinh so block dung cho tuy chon -s va dong total
long long get_entry_blocks(const Entry *e, const Options *opts) {
    long long blocks = e->st.st_blocks; // So block da su dung, moi block 512 byte

    if (opts->size_mode == SIZE_KIB) {
        return (blocks + 1) / 2; // Quy doi sang block 1024 byte
    } else if (opts->size_mode == SIZE_DEFAULT) {
        char *env_bs = getenv("BLOCKSIZE"); 
        if (env_bs) {
            long long bs = atoll(env_bs);
            if (bs > 0) {
                return ((blocks * 512) + bs - 1) / bs;
            }
        }
    }
    return blocks;
}

// Dinh dang thoi gian cua file
void format_time(const Entry *e, const Options *opts, char *buf, size_t size) {
    time_t t = e->st.st_mtime; // Mac dinh lay thoi gian sua doi

    // Chon loai thoi gian theo tuy chon
    if (opts->time_type == TIME_ATIME) t = e->st.st_atime;
    else if (opts->time_type == TIME_CTIME) t = e->st.st_ctime;

    // Chuyen timestamp thanh thoi gian doc duoc
    struct tm *tm_info = localtime(&t);
    if (tm_info == NULL) {
        snprintf(buf, size, "unknown");
        return;
    }

    // File cu hon 6 thang hoac co thoi gian trong tuong lai thi hien thi nam
    time_t now = time(NULL);
    if (t > now || now - t > 182 * 86400) {
        strftime(buf, size, "%b %e  %Y", tm_info);
    } else {
        // File moi hon thi hien thi gio va phut
        strftime(buf, size, "%b %e %H:%M", tm_info);
    }
}
