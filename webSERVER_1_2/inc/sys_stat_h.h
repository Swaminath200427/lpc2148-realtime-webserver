#ifndef SYS_STAT_H
#define SYS_STAT_H

#include <time.h>
#include "custom_time.h"
#include <stdint.h>
#include "unistd.h"
#define _S_IFREG 0x00400000

struct stat {
    time_t st_mtime;
    off_t st_size;
};
struct _stati64 {
    long long st_dev; // Device ID
    long long st_ino; // Inode number
    long long st_mode; // File mode
    long long st_nlink; // Number of hard links
    long long st_uid; // User ID of owner
    long long st_gid; // Group ID of owner
    long long st_rdev; // Device ID of special file
    long long st_size; // Size of file in bytes
    long long st_atime; // Time of last access
    long long st_mtime; // Time of last modification
    long long st_ctime; // Time of last status change � 

    long long st_atimespec; // Time of last access (struct timespec)
    long long st_mtimespec; // Time of last modification (struct timespec)
    long long st_ctimespec; // Time of last status change (struct timespec)
    long long st_blksize; // Block size of filesystem
    long long st_blocks; // Number of 512-byte blocks allocated
};

#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#define S_IFMT 0x000000F0
#define S_IFDIR 0x00400000
#endif // SYS_STAT_H