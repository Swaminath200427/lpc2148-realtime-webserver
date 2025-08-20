#ifndef _PROCESS_H
#define _PROCESS_H

// Define necessary types and constants
typedef int pid_t;

// Declare common functions (replace with actual implementations)
int spawnvp(int mode, const char *path, const char *const argv[]);
pid_t spawnp(int mode, const char *path, char *const argv[]);
int wait(int *status);
int waitpid(pid_t pid, int *status, int options);

// ... other functions as needed

#endif // _PROCESS_H