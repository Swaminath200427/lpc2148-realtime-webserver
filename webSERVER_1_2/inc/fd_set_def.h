#ifndef _FD_SET_DEF_H
#define _FD_SET_DEF_H

#ifndef FD_SETSIZE
#define FD_SETSIZE 64
#endif

#ifndef _FD_SET_DEFINED
#define _FD_SET_DEFINED

typedef struct {
    unsigned long fd_bits[FD_SETSIZE/32];
} fd_set;

#define FD_ZERO(set) do { \
    int i; \
    for(i = 0; i < (FD_SETSIZE/32); i++) \
        ((set)->fd_bits)[i] = 0; \
} while(0)

#define FD_SET(fd, set) do { \
    ((set)->fd_bits[(fd)/32]) |= (1U << ((fd) % 32)); \
} while(0)

#define FD_CLR(fd, set) do { \
    ((set)->fd_bits[(fd)/32]) &= ~(1U << ((fd) % 32)); \
} while(0)

#define FD_ISSET(fd, set) \
    (((set)->fd_bits[(fd)/32] & (1U << ((fd) % 32))) != 0)

#endif /* _FD_SET_DEFINED */

#endif /* _FD_SET_DEF_H */