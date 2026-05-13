#ifndef LIBDIAG_H
#define LIBDIAG_H

#include <sys/types.h>
#include <sys/statvfs.h>

/* 資料結構 */

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

/* proc_reader - 給 bbtop 用 */
int read_proc_stat(pid_t pid, unsigned long *utime, unsigned long *stime);
int read_proc_status(pid_t pid, long *vmrss, char *name, pid_t *ppid);
int read_system_stat(unsigned long long *total, unsigned long long *idle);
pid_t *list_pids(int *count);

/* fs_reader - 給 bbfscheck 用 */
int read_fs_stat(const char *path, struct statvfs *stat);
int scan_dir(const char *path, int max_depth, int *file_count, unsigned long long *total_size);

/* net_reader - 給 bbnetmon 用 */
int read_tcp_connections(const char *path, tcp_conn_t **conn_list, int *count);

/* formatter - 共用輸出 */
void print_table_header(const char **cols, int ncols);
void print_table_row(const char **fields, int nfields);

/* rule_checker - 健康規則判斷 */
int check_fs_health(struct statvfs *stat, int warn_percent);
int check_tcp_anomaly(int state, int count);

#endif
