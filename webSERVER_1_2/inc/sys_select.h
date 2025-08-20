#ifndef _SYS_SELECT_H
#define _SYS_SELECT_H

#include "sys_type.h"
#include "fd_set_def.h"
#include "timeval_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Select function declaration */
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

#ifdef __cplusplus
}
#endif

#endif // _SYS_SELECT_H