#define _NETBSD_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
    else if (S_ISWHT(mode)) str[0] = 'w';
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


// Doi byte sang dang de doc 
void format_human(long long bytes, char *buf, size_t size) {
    const char *suffix[] = {"", "K", "M", "G", "T", "P", "E"};
    double value = (double)bytes;
    int unit = 0;

    while (value >= 1024.0 && unit < 6) {
        value /= 1024.0;
        unit++;
    }

    if (unit == 0) {
        snprintf(buf, size, "%lld", bytes);
    } else if (value >= 10.0) {
        snprintf(buf, size, "%.0f%s", value, suffix[unit]);
    } else {
        snprintf(buf, size, "%.1f%s", value, suffix[unit]);
    }
}

// Dinh dang kich thuoc file cho cot size cua -l
void format_size(const Entry *e, const Options *opts, char *buf, size_t size) {
    if (opts->size_mode == SIZE_HUMAN) {
        format_human((long long)e->st.st_size, buf, size);
    } else {
        snprintf(buf, size, "%lld", (long long)e->st.st_size);
    }
}

// Tinh so block cho -s va dong total (lam tron len)
// Don vi: 512 byte mac dinh, 1024 neu co -k, hoac BLOCKSIZE
long long get_entry_blocks(const Entry *e, const Options *opts) {
    long long bytes = (long long)e->st.st_blocks * 512;
    long long unit = 512;

    if (opts->size_mode == SIZE_KIB) {
        unit = 1024;
    } else if (opts->size_mode == SIZE_DEFAULT) {
        int headerlen;
        long bs;
        (void)getbsize(&headerlen, &bs); // doc bien moi truong BLOCKSIZE
        if (bs > 0) unit = bs;
    }

    return (bytes + unit - 1) / unit;
}

// Dinh dang so block cho tuy chon -s
void format_block_count(const Entry *e, const Options *opts,
                        char *buf, size_t size) {
	if (opts->size_mode == SIZE_HUMAN) {
    		long long bytes = (long long)e->st.st_blocks * 512;
    		format_human(bytes, buf, size);
	} else {
    		snprintf(buf, size, "%lld", get_entry_blocks(e, opts));
	}
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
