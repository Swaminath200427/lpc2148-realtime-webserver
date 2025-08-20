#ifndef BOOL_DEFINED
#define BOOL_DEFINED

#if !defined(__cplusplus) && !defined(__bool_true_false_are_defined)
typedef enum { false = 0, true = 1 } bool;
#endif

#endif // BOOL_DEFINED