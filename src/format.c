#define _NETBSD_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include "format.h"

// Dinh dang loai file va quyen truy cap
void format_mode(mode_t mode, char *str) {
    if (S_ISDIR(mode)) str[0] = 'd';
    else if (S_ISLNK(mode)) str[0] = 'l';
    else if (S_ISCHR(mode)) str[0] = 'c';
    else if (S_ISBLK(mode)) str[0] = 'b';
    else if (S_ISFIFO(mode)) str[0] = 'p';
    else if (S_ISSOCK(mode)) str[0] = 's';
    else str[0] = '-';

    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';

    if (mode & S_ISUID)
        str[3] = (mode & S_IXUSR) ? 's' : 'S';
    else
        str[3] = (mode & S_IXUSR) ? 'x' : '-';

    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';

    if (mode & S_ISGID)
        str[6] = (mode & S_IXGRP) ? 's' : 'S';
    else
        str[6] = (mode & S_IXGRP) ? 'x' : '-';

    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';

    if (mode & S_ISVTX)
        str[9] = (mode & S_IXOTH) ? 't' : 'T';
    else
        str[9] = (mode & S_IXOTH) ? 'x' : '-';

    str[10] = '\0';
}

// Dinh dang kich thuoc file
void format_size(const Entry *e, const Options *opts,
                 char *buf, size_t size) {
    double bytes = (double)e->st.st_size;

    if (opts->size_mode == SIZE_HUMAN) {
        const char *units[] = {"B", "K", "M", "G", "T"};
        int i = 0;

        while (bytes >= 1024 && i < 4) {
            bytes /= 1024;
            i++;
        }

        snprintf(buf, size, "%.1f%s", bytes, units[i]);
    } else if (opts->size_mode == SIZE_KIB) {
        // Doi kich thuoc file sang KiB va lam tron len
        snprintf(buf, size, "%lld",
                 (long long)((e->st.st_size + 1023) / 1024));
    } else {
        snprintf(buf, size, "%lld",
                 (long long)e->st.st_size);
    }
}

// Dinh dang thoi gian theo -c, -u hoac mac dinh
void format_time(const Entry *e, const Options *opts,
                 char *buf, size_t size) {
    time_t t = e->st.st_mtime;

    if (opts->time_type == TIME_ATIME)
        t = e->st.st_atime;
    else if (opts->time_type == TIME_CTIME)
        t = e->st.st_ctime;

    struct tm *tm_info = localtime(&t);

    // Xu ly truong hop khong chuyen doi duoc thoi gian
    if (tm_info == NULL ||
        strftime(buf, size, "%b %e %H:%M", tm_info) == 0) {
        snprintf(buf, size, "unknown");
    }
}


