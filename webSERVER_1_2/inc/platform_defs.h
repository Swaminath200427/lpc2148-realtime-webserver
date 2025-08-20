#ifndef PLATFORM_DEFS_H
#define PLATFORM_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Use different structure names to avoid conflicts */
struct mg_timeval {
    long tv_sec;     /* seconds */
    long tv_usec;    /* microseconds */
};

struct mg_timezone {
    int tz_minuteswest;     /* minutes west of Greenwich */
    int tz_dsttime;         /* type of DST correction */
};

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_DEFS_H