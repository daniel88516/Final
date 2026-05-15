#ifndef DIAG_COMMON_H
#define DIAG_COMMON_H

#include <sys/types.h>
#include <sys/statvfs.h>

typedef struct {
    pid_t pid;
    pid_t ppid;
    char name[256];
    unsigned long utime;
    unsigned long stime;
    long vmrss;  /* KB */
} proc_info_t;

typedef struct {
    unsigned int local_addr;
    unsigned short local_port;
    unsigned int remote_addr;
    unsigned short remote_port;
    int state;
} tcp_conn_t;

#endif
