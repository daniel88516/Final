#include "proc_reader.h"
#include "formatter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

int read_proc_stat(pid_t pid, unsigned long *utime, unsigned long *stime)
{
    char path[64];
    FILE *f;
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

int diag_print_process_summary(void)
{
    unsigned long long total1, idle1, total2, idle2;
    pid_t *pids;
    int count, i;
    static const char *hdr[] = { "PID", "PPID", "NAME", "CPU%", "RSS(KB)" };

    if (read_system_stat(&total1, &idle1) != 0) {
        fprintf(stderr, "bbtop: cannot read /proc/stat\n");
        return 1;
    }
    /* 採樣間隔 200ms，讓 CPU% 有意義 */
    usleep(200000);
    if (read_system_stat(&total2, &idle2) != 0) {
        fprintf(stderr, "bbtop: cannot read /proc/stat\n");
        return 1;
    }

    unsigned long long dtotal = total2 - total1;
    unsigned long long didle  = idle2  - idle1;
    int cpu_pct = (dtotal > 0) ? (int)((dtotal - didle) * 100 / dtotal) : 0;

    printf("System CPU usage: %d%%\n\n", cpu_pct);
    print_table_header(hdr, 5);

    pids = list_pids(&count);
    if (!pids) {
        fprintf(stderr, "bbtop: cannot list /proc\n");
        return 1;
    }

    for (i = 0; i < count; i++) {
        unsigned long utime = 0, stime = 0;
        long vmrss = 0;
        char name[256] = "?";
        pid_t ppid = 0;
        char pid_s[16], ppid_s[16], cpu_s[16], rss_s[16];
        const char *row[5];

        read_proc_stat(pids[i], &utime, &stime);
        read_proc_status(pids[i], &vmrss, name, &ppid);

        snprintf(pid_s,  sizeof(pid_s),  "%d", pids[i]);
        snprintf(ppid_s, sizeof(ppid_s), "%d", ppid);
        snprintf(cpu_s,  sizeof(cpu_s),  "-");
        snprintf(rss_s,  sizeof(rss_s),  "%ld", vmrss);

        row[0] = pid_s; row[1] = ppid_s; row[2] = name;
        row[3] = cpu_s; row[4] = rss_s;
        print_table_row(row, 5);
    }

    free(pids);
    return 0;
}
