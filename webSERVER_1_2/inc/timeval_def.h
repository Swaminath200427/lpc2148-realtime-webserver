#ifndef _TIMEVAL_DEF_H
#define _TIMEVAL_DEF_H

#include "sys_type.h"

#ifndef _TIMEVAL_DEFINED
#define _TIMEVAL_DEFINED
struct timeval {
    time_t tv_sec;
    suseconds_t tv_usec;
};
#endif

#ifndef _TIMEZONE_DEFINED
#define _TIMEZONE_DEFINED
struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};
#endif

#endif // _TIMEVAL_DEF_H