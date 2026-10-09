#ifndef OPTIONS_H
#define OPTIONS_H

// Kieu thoi gian
typedef enum {
    TIME_MTIME, // -t: Thoi gian sua doi
    TIME_CTIME, // -c: Thoi gian thay doi
    TIME_ATIME  // -u: Thoi gian truy cap
} TimeType;

// Kieu dung luong
typedef enum {
    SIZE_DEFAULT,
    SIZE_KIB,   // -k: Dung luong KiB
    SIZE_HUMAN  // -h: Dung luong de doc
} SizeMode;

// Kieu hien thi ky tu
typedef enum {
    CHAR_PRINTABLE, // -q
    CHAR_RAW        // -w
} CharMode;

// Cac tuy chon cua lenh ls
typedef struct {
    int show_almost_all; // -A
    int show_all;        // -a
    int dir_as_file;     // -d
    int classify;        // -F
    int unsorted;        // -f
    int show_inode;      // -i
    int long_format;     // -l
    int numeric_uid_gid; // -n
    int recursive;       // -R
    int reverse_sort;    // -r
    int sort_size;       // -S
    int show_blocks;     // -s
    int sort_time;       // -t

    TimeType time_type;  // -c, -u
    SizeMode size_mode;  // -h, -k
    CharMode char_mode;  // -q, -w
} Options;

// Khoi tao va doc tuy chon
void init_options(Options *opts);
int parse_options(int argc, char *argv[], Options *opts);

#endif
