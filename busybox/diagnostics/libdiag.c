#include "libdiag.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

/* ─── proc_reader ─────────────────────────────────────── */

/* 讀取 /proc/[pid]/stat，取得 utime 與 stime */
int read_proc_stat(pid_t pid, unsigned long *utime, unsigned long *stime)
{
    char path[64];
    FILE *f;
    /* /proc/[pid]/stat 欄位：pid comm state ppid ... utime(14) stime(15) */
    unsigned long dummy_ul;
    int dummy_i;
    char comm[256], state;

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    f = fopen(path, "r");
    if (!f) return -1;

    fscanf(f, "%d %s %c %d %d %d %d %d %lu %lu %lu %lu %lu %lu %lu",
           &dummy_i, comm, &state,
           &dummy_i, &dummy_i, &dummy_i, &dummy_i, &dummy_i,
           &dummy_ul, &dummy_ul, &dummy_ul, &dummy_ul, &dummy_ul,
           utime, stime);
    fclose(f);
    return 0;
}

/* 讀取 /proc/[pid]/status，取得 VmRSS、Name、PPid */
int read_proc_status(pid_t pid, long *vmrss, char *name, pid_t *ppid)
{
    char path[64], line[256];
    FILE *f;

    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    f = fopen(path, "r");
    if (!f) return -1;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "Name:", 5) == 0)
            sscanf(line, "Name:\t%255s", name);
        else if (strncmp(line, "PPid:", 5) == 0)
            sscanf(line, "PPid:\t%d", ppid);
        else if (strncmp(line, "VmRSS:", 6) == 0)
            sscanf(line, "VmRSS:\t%ld", vmrss);
    }
    fclose(f);
    return 0;
}

/* 讀取 /proc/stat，取得系統整體 CPU total 與 idle */
int read_system_stat(unsigned long long *total, unsigned long long *idle)
{
    FILE *f;
    unsigned long long user, nice, system, idle_val, iowait, irq, softirq;

    f = fopen("/proc/stat", "r");
    if (!f) return -1;

    fscanf(f, "cpu %llu %llu %llu %llu %llu %llu %llu",
           &user, &nice, &system, &idle_val, &iowait, &irq, &softirq);
    fclose(f);

    *idle  = idle_val;
    *total = user + nice + system + idle_val + iowait + irq + softirq;
    return 0;
}

/* 列出 /proc 下所有數字目錄（即 PID 列表） */
pid_t *list_pids(int *count)
{
    DIR *d;
    struct dirent *ent;
    pid_t *pids = NULL;
    int n = 0;

    d = opendir("/proc");
    if (!d) return NULL;

    while ((ent = readdir(d)) != NULL) {
        if (ent->d_name[0] >= '1' && ent->d_name[0] <= '9') {
            pids = realloc(pids, (n + 1) * sizeof(pid_t));
            pids[n++] = atoi(ent->d_name);
        }
    }
    closedir(d);
    *count = n;
    return pids;
}

/* ─── fs_reader ───────────────────────────────────────── */

/* 取得指定路徑的檔案系統統計資訊 */
int read_fs_stat(const char *path, struct statvfs *stat)
{
    return statvfs(path, stat);
}

/* 遞迴掃描目錄，統計檔案數與總大小 */
int scan_dir(const char *path, int max_depth, int *file_count, unsigned long long *total_size)
{
    DIR *d;
    struct dirent *ent;
    struct stat st;
    char fullpath[4096];

    if (max_depth < 0) return 0;

    d = opendir(path);
    if (!d) return -1;

    while ((ent = readdir(d)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, ent->d_name);

        /* 跳過虛擬檔案系統 */
        if (strcmp(fullpath, "/proc") == 0 ||
            strcmp(fullpath, "/sys")  == 0 ||
            strcmp(fullpath, "/dev")  == 0) continue;

        if (lstat(fullpath, &st) != 0) continue;

        if (S_ISDIR(st.st_mode)) {
            scan_dir(fullpath, max_depth - 1, file_count, total_size);
        } else {
            (*file_count)++;
            *total_size += st.st_size;
        }
    }
    closedir(d);
    return 0;
}

/* ─── net_reader ──────────────────────────────────────── */

/* 解析 /proc/net/tcp 或 /proc/net/tcp6 */
int read_tcp_connections(const char *path, tcp_conn_t **conn_list, int *count)
{
    FILE *f;
    char line[256];
    tcp_conn_t *conns = NULL;
    int n = 0;
    unsigned int local_addr, remote_addr, state;
    unsigned int local_port, remote_port;

    f = fopen(path, "r");
    if (!f) return -1;

    fgets(line, sizeof(line), f);  /* 跳過 header */

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, " %*d: %X:%X %X:%X %X",
                   &local_addr, &local_port,
                   &remote_addr, &remote_port,
                   &state) == 5) {
            conns = realloc(conns, (n + 1) * sizeof(tcp_conn_t));
            conns[n].local_addr   = local_addr;
            conns[n].local_port   = (unsigned short)local_port;
            conns[n].remote_addr  = remote_addr;
            conns[n].remote_port  = (unsigned short)remote_port;
            conns[n].state        = (int)state;
            n++;
        }
    }
    fclose(f);
    *conn_list = conns;
    *count = n;
    return 0;
}

/* ─── formatter ───────────────────────────────────────── */

void print_table_header(const char **cols, int ncols)
{
    for (int i = 0; i < ncols; i++)
        printf("%-15s", cols[i]);
    printf("\n");
    for (int i = 0; i < ncols * 15; i++)
        printf("-");
    printf("\n");
}

void print_table_row(const char **fields, int nfields)
{
    for (int i = 0; i < nfields; i++)
        printf("%-15s", fields[i]);
    printf("\n");
}

/* ─── rule_checker ────────────────────────────────────── */

/* 檢查檔案系統使用率是否超過門檻，超過回傳 1 */
int check_fs_health(struct statvfs *stat, int warn_percent)
{
    unsigned long used = stat->f_blocks - stat->f_bfree;
    int percent = (int)(used * 100 / stat->f_blocks);
    return percent >= warn_percent ? 1 : 0;
}

/* 檢查 TCP 連線是否異常（TIME_WAIT 或 CLOSE_WAIT 過多） */
int check_tcp_anomaly(int state, int count)
{
    /* state 6 = TIME_WAIT, state 8 = CLOSE_WAIT */
    if ((state == 6 || state == 8) && count > 100)
        return 1;
    return 0;
}
