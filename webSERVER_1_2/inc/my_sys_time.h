#ifndef MY_SYS_TIME_H
#define MY_SYS_TIME_H

/* Include system time definitions first */
#include <sys/time.h>
#include <sys/types.h>

/* Then include our platform definitions */
#include "platform_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Only declare the function prototype if we're compiling mongoose.c */
#ifdef MONGOOSE_IMPLEMENTATION
static int mg_gettimeofday(struct timeval *tv, struct timezone *tz);
#define gettimeofday mg_gettimeofday
#endif

#ifdef __cplusplus
}
#endif

#endif // MY_SYS_TIME_H