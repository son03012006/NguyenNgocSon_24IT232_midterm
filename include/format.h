#ifndef FORMAT_H
#define FORMAT_H

#include <sys/stat.h>
#include <stddef.h>
#include "entry.h"
#include "options.h"

// Dinh dang quyen truy cap file
void format_mode(mode_t mode, char *str);

// Doi so byte thanh dang de doc (vi du 300K) bang humanize_number(3)
void format_human(long long bytes, char *buf, size_t size);

// Dinh dang dung luong file (cot size cua -l)
void format_size(const Entry *e, const Options *opts,
                 char *buf, size_t size);

// So block cua mot entry (dung cho -s va dong total), ton trong -k va BLOCKSIZE
long long get_entry_blocks(const Entry *e, const Options *opts);

// Chuoi cho cot cua -s: so block, hoac kich thuoc de doc neu co -h
void format_block_count(const Entry *e, const Options *opts,
                        char *buf, size_t size);

// Dinh dang thoi gian file
void format_time(const Entry *e, const Options *opts,
                 char *buf, size_t size);

#endif
