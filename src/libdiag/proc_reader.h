#ifndef PROC_READER_H
#define PROC_READER_H

#include "diag_common.h"

int    read_proc_stat(pid_t pid, unsigned long *utime, unsigned long *stime);
int    read_proc_status(pid_t pid, long *vmrss, char *name, pid_t *ppid);
int    read_system_stat(unsigned long long *total, unsigned long long *idle);
pid_t *list_pids(int *count);

int    diag_print_process_summary(void);

#endif
