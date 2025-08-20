// portable_restrict.h
#ifndef PORTABLE_RESTRICT_H
#define PORTABLE_RESTRICT_H

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    // C99 or later: use the standard restrict keyword
    #define PORTABLE_RESTRICT restrict
#elif defined(__GNUC__) || defined(__clang__)
    // GCC and Clang support __restrict as an extension
    #define PORTABLE_RESTRICT __restrict
#else
    // Other compilers: remove restrict completely
    #define PORTABLE_RESTRICT
#endif

#endif // PORTABLE_RESTRICT_H
