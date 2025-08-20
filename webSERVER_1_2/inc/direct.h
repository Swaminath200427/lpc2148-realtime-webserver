#ifndef PROJECT_DIRECT_H  // Add header guard
#define PROJECT_DIRECT_H

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>  // This includes the system direct.h, not your local one
#else
// Include alternative headers or define equivalent functions for your platform
#include <sys/stat.h>  // Common alternative for directory operations
#include <unistd.h>    // Common alternative for directory operations
#endif

#endif // PROJECT_DIRECT_H