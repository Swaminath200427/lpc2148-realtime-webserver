#ifndef _UNISTD_H
#define _UNISTD_H
#include "sys_type.h"
#include <sys/types.h>  // for mode_t definition

// Define necessary types and constants
typedef long off_t;
typedef int pid_t;
typedef int uid_t;
typedef int gid_t;

// Declare common functions (replace with actual implementations)
int access(const char *pathname, int mode);
int chdir(const char *path);
int chmod(const char *path, mode_t mode);
int chown(const char *path, uid_t owner, gid_t group);
int close(int fd);
int dup(int oldfd);
int dup2(int oldfd, int newfd);
int execve(const char *path, char *const argv[], char *const envp[]);
pid_t fork(void);
int geteuid(void);
int getgid(void);
pid_t getpid(void);
uid_t getuid(void);
int link(const char *oldpath, const char *newpath);
int mkdir(const char *path, mode_t mode);
int pipe(int pipefd[2]);
int read(int fd, void *buf, size_t count);
int rmdir(const char *path);
int sleep(unsigned int seconds);
int symlink(const char *target, const char *linkname);
int unlink(const char *path);
int write(int fd, const void *buf, size_t count);

// ... other functions as needed
#ifdef __cplusplus
extern "C" {
#endif

int chmod(const char *path, mode_t mode);
int mkdir(const char *path, mode_t mode);

/* Add other unistd.h declarations as needed */

#ifdef __cplusplus
}
#endif


#endif // _UNISTD_H