#ifndef WS2TCPIP_WRAPPER_H_INCLUDED  // Choose a unique name for the guard
#define WS2TCPIP_WRAPPER_H_INCLUDED

#if defined(_WIN32) || defined(_WIN64)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif

#endif // WS2TCPIP_WRAPPER_H_INCLUDED