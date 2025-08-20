#ifndef SYS_STAT_H
#define SYS_STAT_H

#include <time.h>

struct stat {
    time_t st_mtime;
    off_t st_size;
};

#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)

#endif // SYS_STAT_H