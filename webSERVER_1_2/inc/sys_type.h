#ifndef _SYS_TYPE_H
#define _SYS_TYPE_H

/* Include fd_set definitions first */
#include "fd_set_def.h"

/* Then include system types */
#include <sys/types.h>

#ifndef _TIME_T_DEFINED
typedef long time_t;
#define _TIME_T_DEFINED
#endif

#ifndef _SUSECONDS_T_DEFINED
typedef long suseconds_t;
#define _SUSECONDS_T_DEFINED
#endif

/* Common permission bit definitions */
#ifndef _MODE_BITS_DEFINED
#define _MODE_BITS_DEFINED
#define S_IRWXU 0700
#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100
#define S_IRWXG 0070
#define S_IRGRP 0040
#define S_IWGRP 0020
#define S_IXGRP 0010
#define S_IRWXO 0007
#define S_IROTH 0004
#define S_IWOTH 0002
#define S_IXOTH 0001
#define S_ISUID 0004000
#define S_ISGID 0002000
#define S_ISVTX 0001000
#endif

#endif // _SYS_TYPE_H