#ifndef _WINDOWS_H
#define _WINDOWS_H
#define INVALID_HANDLE_VALUE ((HANDLE)-1)
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

// Basic types
typedef unsigned short WORD;
typedef  uint32_t DWORD;
typedef void* HANDLE;
typedef HANDLE HWND;
typedef unsigned long long UINT64;
typedef unsigned char BYTE;
typedef char CHAR;
typedef int BOOL;
typedef unsigned int UINT;
typedef long LONG;
typedef uint32_t DWORD;
// Window handles
//typedef HWND* HWND;

// Character pointers
typedef const wchar_t *LPCWSTR;
typedef wchar_t *LPWSTR;

// Message types
typedef UINT UINT_PTR;
typedef UINT_PTR WPARAM;
typedef LONG LPARAM;
typedef wchar_t WCHAR;
// System information
typedef struct _SYSTEM_INFO {
    union {
        DWORD dwOemIdentifier;
        BYTE Byte[12];
    } uIdentifier;
    DWORD dwPageSize;
    DWORD dwAllocationGranularity;
    DWORD dwMinimumPhysicalAddress;
    DWORD dwMaximumPhysicalAddress;
    DWORD dwActiveProcessorCount;
    DWORD dwNumberOfProcessors;
    DWORD dwProcessorType;
    DWORD dwAllocationGranularity64;
    DWORD dwCacheLineSize;
    DWORD dwPageTableEntries;
    DWORD dwNumberOfPhysicalPages;
    DWORD dwReserved1;
    DWORD dwReserved2;
    DWORD dwReserved3;
} SYSTEM_INFO, *PSYSTEM_INFO;
typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME;
typedef struct _WIN32_FIND_DATAW {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    WCHAR cFileName[260];
    WCHAR cAlternateFileName[14];
} WIN32_FIND_DATAW;

// MessageBox function
int MessageBox(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType);
#define TRUE 1
#define FALSE 0
#define NULL ((void*)0)
// Other common functions
// ... (add other functions as needed)

#endif // _WINDOWS_H