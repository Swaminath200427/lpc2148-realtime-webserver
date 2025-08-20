#ifndef CUSTOM_ERRNO_H
#define CUSTOM_ERRNO_H

#include <errno.h>      // Include the standard errno.h
#include "sys_socket.h" // For socket functions
#include "netinet_in.h" // For sockaddr_in
#include "arpa_inet.h"  // For inet_pton, inet_ntop

// Define MG_SOCKET_TYPE if not already defined
#ifndef MG_SOCKET_TYPE
typedef int MG_SOCKET_TYPE;
#endif

// Macro to extract file descriptor from mg_connection
#define FD(c) ((c)->fd)

// Structure for mg_connection (if not defined elsewhere)
/*struct mg_connection {
    MG_SOCKET_TYPE fd;
    // Add other necessary fields
};

// Union for IPv4 and IPv6 addresses
union usa {
    struct sockaddr sa;
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;
};
*/
// Function to set local address
static int setlocaddr(MG_SOCKET_TYPE fd, struct mg_addr *addr);

/* Function for connecting a socket
static int mg_connect_socket(struct mg_connection *c, const union usa *usa, socklen_t slen) {
    int rc;
    errno = 0;  // Reset errno before the call
    rc = connect(FD(c), &usa->sa, slen);
    return rc;
}
*/
// Error checking macro
#define CHECK_ERROR(expr) \
    do { \
        if ((expr) < 0) { \
            fprintf(stderr, "Error: %s\n", strerror(errno)); \
            return -1; \
        } \
    } while (0)

#endif // CUSTOM_ERRNO_H