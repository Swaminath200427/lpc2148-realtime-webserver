#ifndef SYS_SOCKET_H
#define SYS_SOCKET_H

// Include standard C libraries for socket programming
#include "sys_type.h"    // Defines data types used in system calls
#include "netinet_in.h"   // Definitions for the Internet Protocol family (struct sockaddr_in, sockaddr_in6)
#include "arpa_inet.h"    // Functions for manipulating numeric IP addresses (e.g., inet_pton, inet_ntop)
#include "unistd.h"       // For close() function to close sockets
#include <errno.h>        // For error number definitions (e.g., errno)
#include <stdbool.h>      // For boolean types (bool, true, false)
#include <string.h>       // For functions like memset(), memcpy()
#include "mongoose.h"

// Ensure the sa_family_t type is defined, typically it's in sys/socket.h
#ifndef sa_family_t
typedef uint16_t sa_family_t;  // Defining sa_family_t as uint16_t if not already defined
#endif

// Define any custom macros or constants if necessary
#define MSG_NONBLOCKING 0x800  // Custom flag for non-blocking send (adjust as per your needs)

// Structure representing an IP address (for both IPv4 and IPv6)
/*#ifndef MG_ADDR_DEFINED
#define MG_ADDR_DEFINED
struct mg_addr {
    struct sockaddr sa;   // Structure representing socket address
    socklen_t len;        // Length of the address
    sa_family_t sa_family;  // Address family (AF_INET or AF_INET6)
    uint16_t port;        // Port number (in network byte order)
    char ip[16];          // IP address (IPv4 or IPv6)
};

#ifndef MG_CONNECTION_DEFINED
#define MG_CONNECTION_DEFINED
// Structure representing a connection
struct mg_connection {
    int is_udp;             // Boolean flag indicating if the connection is UDP
    int fd;                 // File descriptor representing the socket
    struct mg_addr rem;     // Remote address (for sending/receiving data)
    struct mg_addr loc;     // Local address (for sending/receiving data)
};
*/
// Helper macro to extract the file descriptor from connection struct
#define FD(c) ((c)->fd)

// Union for holding IPv4 and IPv6 addresses
union usa {
    struct sockaddr sa;
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;
};

// Example function declarations
static int setlocaddr(int fd, struct mg_addr *addr);  // Example function for setting local address
socklen_t tousa(struct mg_addr *a, union usa *usa);  // Helper function for address conversion

//#endif  // MG_CONNECTION_DEFINED
//#endif	//
#endif  // SYS_SOCKET_H
