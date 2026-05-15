#ifndef FS_READER_H
#define FS_READER_H

#include "diag_common.h"

int read_fs_stat(const char *path, struct statvfs *stat);
int scan_dir(const char *path, int max_depth, int *file_count, unsigned long long *total_size);

int diag_print_filesystems(void);

#endif
