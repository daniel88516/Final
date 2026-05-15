#include "fs_reader.h"
#include "formatter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int read_fs_stat(const char *path, struct statvfs *stat)
{
    return statvfs(path, stat);
}

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

        if (strcmp(fullpath, "/proc") == 0 || strcmp(fullpath, "/sys") == 0 ||
            strcmp(fullpath, "/dev")  == 0)
            continue;

        if (lstat(fullpath, &st) != 0) continue;

        if (S_ISDIR(st.st_mode)) {
            scan_dir(fullpath, max_depth - 1, file_count, total_size);
        } else {
            (*file_count)++;
            *total_size += (unsigned long long)st.st_size;
        }
    }
    closedir(d);
    return 0;
}

int diag_print_filesystems(void)
{
    FILE *f;
    char line[512], dev[128], mount[256], fstype[64], opts[128];
    int dummy_i;
    static const char *hdr[] = { "FILESYSTEM", "SIZE(MB)", "USED(MB)", "AVAIL(MB)", "USE%", "MOUNT", "STATUS" };

    f = fopen("/proc/mounts", "r");
    if (!f) {
        fprintf(stderr, "bbfscheck: cannot open /proc/mounts\n");
        return 1;
    }

    print_table_header(hdr, 7);

    while (fgets(line, sizeof(line), f)) {
        struct statvfs st;
        unsigned long long total_bytes, used_bytes, avail_bytes;
        unsigned long size_mb, used_mb, avail_mb;
        int use_pct;
        char size_s[16], used_s[16], avail_s[16], pct_s[16];
        const char *row[7];
        const char *status;

        if (sscanf(line, "%127s %255s %63s %127s %d %d",
                   dev, mount, fstype, opts, &dummy_i, &dummy_i) < 2)
            continue;

        /* 跳過虛擬 FS，只顯示真實掛載點 */
        if (strcmp(fstype, "proc") == 0   || strcmp(fstype, "sysfs") == 0  ||
            strcmp(fstype, "devtmpfs") == 0 || strcmp(fstype, "tmpfs") == 0 ||
            strcmp(fstype, "cgroup") == 0 || strcmp(fstype, "cgroup2") == 0 ||
            strcmp(fstype, "devpts") == 0 || strcmp(fstype, "debugfs") == 0 ||
            strcmp(fstype, "securityfs") == 0)
            continue;

        if (read_fs_stat(mount, &st) != 0) continue;
        if (st.f_blocks == 0) continue;

        total_bytes = (unsigned long long)st.f_blocks * st.f_frsize;
        avail_bytes = (unsigned long long)st.f_bavail * st.f_frsize;
        used_bytes  = total_bytes - (unsigned long long)st.f_bfree * st.f_frsize;

        size_mb  = (unsigned long)(total_bytes / (1024 * 1024));
        used_mb  = (unsigned long)(used_bytes  / (1024 * 1024));
        avail_mb = (unsigned long)(avail_bytes / (1024 * 1024));
        use_pct  = (int)(used_bytes * 100 / total_bytes);

        snprintf(size_s,  sizeof(size_s),  "%lu", size_mb);
        snprintf(used_s,  sizeof(used_s),  "%lu", used_mb);
        snprintf(avail_s, sizeof(avail_s), "%lu", avail_mb);
        snprintf(pct_s,   sizeof(pct_s),   "%d%%", use_pct);

        status = check_fs_health(&st, 80) ? "WARN" : "OK";

        row[0] = dev;    row[1] = size_s;  row[2] = used_s;
        row[3] = avail_s; row[4] = pct_s;  row[5] = mount;
        row[6] = status;
        print_table_row(row, 7);
    }

    fclose(f);
    return 0;
}
