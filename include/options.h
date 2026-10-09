#ifndef OPTIONS_H
#define OPTIONS_H

// Quan ly loai thoi gian
typedef enum {
    TIME_MTIME, // Thoi gian sua doi
    TIME_CTIME, // -c: Thoi gian thay doi trang thai
    TIME_ATIME  // -u: Thoi gian truy cap
} TimeType;

// Kieu hien thi kich thuoc
typedef enum {
    SIZE_DEFAULT, // Mac dinh
    SIZE_KIB,     // -k: Kilobyte
    SIZE_HUMAN    // -h: De doc
} SizeMode;

// Cach in ky tu
typedef enum {
    CHAR_PRINTABLE, // -q: Thay ky tu bang '?'
    CHAR_RAW        // -w: In ky tu goc
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

    TimeType time_type;
    SizeMode size_mode;
    CharMode char_mode;
} Options;

// Khoi tao va phan tich tuy chon
void init_options(Options *opts);
int parse_options(int argc, char *argv[], Options *opts);

#endif
