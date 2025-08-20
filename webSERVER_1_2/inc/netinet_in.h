#ifndef _NETINET_IN_H
#define _NETINET_IN_H

#include <stdint.h>         // For uint16_t, uint32_t
//#include "sys_socket.h"     // For sa_family_t and other socket-related definitions

// Define sa_family_t if not already defined
#ifndef sa_family_t
typedef uint16_t sa_family_t;  // Typically an unsigned short or uint16_t
#endif

// IPv6 address
struct in6_addr {
    unsigned char s6_addr[16];  // 128-bit IPv6 address
};

// sockaddr_in6 structure (for IPv6 addresses)
struct sockaddr_in6 {
    sa_family_t sin6_family;    // Address family (AF_INET6)
    uint16_t sin6_port;         // Port number (network byte order)
    uint32_t sin6_flowinfo;     // IPv6 flow information
    struct in6_addr sin6_addr;  // IPv6 address
    uint32_t sin6_scope_id;     // Scope ID (e.g., interface index)
};

#endif  // _NETINET_IN_H
