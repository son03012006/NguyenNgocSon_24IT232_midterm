#ifndef FORMAT_H
#define FORMAT_H

#include <sys/stat.h>
#include <stddef.h>
#include "entry.h"
#include "options.h"

// Dinh dang quyen truy cap file
void format_mode(mode_t mode, char *str);

// Dinh dang dung luong file
void format_size(const Entry *e, const Options *opts,
                 char *buf, size_t size);

// Dinh dang thoi gian file
void format_time(const Entry *e, const Options *opts,
                 char *buf, size_t size);

// Lay ky tu phan loai file theo -F
char get_classify_suffix(mode_t mode);

#endif
