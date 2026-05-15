#ifndef NET_READER_H
#define NET_READER_H

#include "diag_common.h"

int read_tcp_connections(const char *path, tcp_conn_t **conn_list, int *count);

int diag_print_tcp_summary(void);

#endif
