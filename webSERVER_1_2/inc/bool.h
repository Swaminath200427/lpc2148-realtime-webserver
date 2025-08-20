#ifndef CUSTOM_BOOL_H
#define CUSTOM_BOOL_H

#ifndef __cplusplus  // In C++, bool is already a keyword

#ifndef __bool_true_false_are_defined  // Avoid redefining if already defined

typedef enum {
    false = 0,
    true = 1
} bool;

#define __bool_true_false_are_defined 1

#endif // __bool_true_false_are_defined

#endif // __cplusplus

#endif // CUSTOM_BOOL_H