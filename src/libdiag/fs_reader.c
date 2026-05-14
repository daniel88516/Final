#include "fs_reader.h"

#include <stdio.h>
#include <sys/statvfs.h>

int diag_print_filesystems(void)
{
    struct statvfs vfs;

    if (statvfs("/", &vfs) != 0) {
        perror("bbfscheck: statvfs");
        return 1;
    }

    printf("mount used_blocks free_blocks\n");
    printf("/ %lu %lu\n",
           (unsigned long)(vfs.f_blocks - vfs.f_bfree),
           (unsigned long)vfs.f_bfree);
    return 0;
}
