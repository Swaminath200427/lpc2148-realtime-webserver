#ifndef _WINSOCK2_H
#define _WINSOCK2_H
#define WSADESCRIPTION_LEN 256
#define WSASYSSTATUS_INFO_LEN 128
#define MAKEWORD(lo, hi) (((WORD)((hi) << 8)) | (WORD)(lo))  // Fixed macro#define IPPROTO_TCP 6
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17
#define SOL_SOCKET 6
#define SO_EXCLUSIVEADDRUSE 28
#define SO_REUSEADDR 2
#define TCP_NODELAY 1
#define SO_KEEPALIVE 8
#define SOCK_DGRAM 2
#define SOCK_STREAM 1
#include <stdio.h>
#include <stdint.h>
#include "windows.h"
#include "ws2tcpip.h"

// Define necessary types and constants;
typedef int SOCKET;
typedef int socklen_t;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef struct sockaddr sockaddr;
typedef struct in_addr in_addr;
int getsockname(SOCKET s, struct sockaddr *name, socklen_t *namelen);
struct in_addr {
    unsigned long s_addr;
};
struct sockaddr_in {
    short sin_family;
    u_short sin_port;
    in_addr sin_addr;
};
struct sockaddr {
    short sin_family;
    u_short sin_port;
    in_addr sin_addr;
};
typedef struct WSAData {
    WORD wVersion;
    WORD wHighVersion;
    WORD wLowVersion;
    WORD wReserved;
    CHAR szDescription[WSADESCRIPTION_LEN + 1];
    CHAR szSystemStatus[WSASYSSTATUS_INFO_LEN + 1];
    WORD iMaxSockets;
    WORD iMaxUdpDg;
    WORD wDefaultTOS;
    WORD wDefaultTTL;
    WORD wReserved2;
    WORD wReserved3;
    WORD wReserved4;
    WORD wReserved5;
    WORD wReserved6;
    WORD wReserved7;
    WORD wReserved8;
    WORD wReserved9;
    WORD wReserved10;
} WSADATA, *LPWSADATA;

// Declare common functions (replace with actual implementations)
int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
int WSACleanup(void);
SOCKET socket(int af, int type, int protocol);
int bind(SOCKET s, const sockaddr *name, int namelen);
int listen(SOCKET s, int backlog);
SOCKET accept(SOCKET s, sockaddr *addr, int *addrlen);
int connect(SOCKET s, const sockaddr *name, int namelen);
int send(SOCKET s, const char *buf, int len, int flags);
int recv(SOCKET s, char *buf, int len, int flags);
int closesocket(SOCKET s);
int shutdown(SOCKET s, int how);
// ... other functions as needed

#endif // _WINSOCK2_H