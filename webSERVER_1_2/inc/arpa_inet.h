#ifndef _ARPA_INET_H
#define _ARPA_INET_H

#include "netinet_in.h" // For the in_addr and sockaddr_in structures
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
// Convert IPv4 and IPv6 addresses from text to binary form
int inet_pton(int af, const char *src, void *dst);

// Convert IPv4 and IPv6 addresses from binary to text form
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);

// Convert an IPv4 address from network byte order to host byte order (text format)
char *inet_ntoa(struct in_addr in);

/*// Convert a string IPv4 address to an integer in network byte order
in_addr_t inet_addr(const char *cp);*/
#ifndef in_addr_t
typedef uint32_t in_addr_t;  // IPv4 addresses are typically 32-bit unsigned integers
#endif
#endif  // _ARPA_INET_H
