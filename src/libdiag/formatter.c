#include "formatter.h"
#include <stdio.h>

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

int check_fs_health(struct statvfs *stat, int warn_percent)
{
    if (stat->f_blocks == 0) return 0;
    unsigned long used = stat->f_blocks - stat->f_bfree;
    int percent = (int)(used * 100 / stat->f_blocks);
    return percent >= warn_percent ? 1 : 0;
}

/* state 6 = TIME_WAIT, state 8 = CLOSE_WAIT */
int check_tcp_anomaly(int state, int count)
{
    return ((state == 6 || state == 8) && count > 100) ? 1 : 0;
}
