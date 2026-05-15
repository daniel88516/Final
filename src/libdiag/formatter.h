#ifndef FORMATTER_H
#define FORMATTER_H

#include "diag_common.h"

void print_table_header(const char **cols, int ncols);
void print_table_row(const char **fields, int nfields);
int  check_fs_health(struct statvfs *stat, int warn_percent);
int  check_tcp_anomaly(int state, int count);

#endif
