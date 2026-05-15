#include "net_reader.h"
#include "formatter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *tcp_state_name(int state)
{
    static const char *names[] = {
        "UNKNOWN",    /* 0 */
        "ESTABLISHED",/* 1 */
        "SYN_SENT",   /* 2 */
        "SYN_RECV",   /* 3 */
        "FIN_WAIT1",  /* 4 */
        "FIN_WAIT2",  /* 5 */
        "TIME_WAIT",  /* 6 */
        "CLOSE",      /* 7 */
        "CLOSE_WAIT", /* 8 */
        "LAST_ACK",   /* 9 */
        "LISTEN",     /* A=10 */
        "CLOSING",    /* B=11 */
    };
    if (state < 0 || state > 11) return "UNKNOWN";
    return names[state];
}

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

    fgets(line, sizeof(line), f);  /* skip header */

    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, " %*d: %X:%X %X:%X %X",
                   &local_addr, &local_port,
                   &remote_addr, &remote_port,
                   &state) == 5) {
            conns = realloc(conns, (n + 1) * sizeof(tcp_conn_t));
            conns[n].local_addr  = local_addr;
            conns[n].local_port  = (unsigned short)local_port;
            conns[n].remote_addr = remote_addr;
            conns[n].remote_port = (unsigned short)remote_port;
            conns[n].state       = (int)state;
            n++;
        }
    }
    fclose(f);
    *conn_list = conns;
    *count = n;
    return 0;
}

int diag_print_tcp_summary(void)
{
    tcp_conn_t *conns4 = NULL, *conns6 = NULL;
    int count4 = 0, count6 = 0;
    int state_counts[12] = {0};
    int total, i;
    static const char *hdr[] = { "STATE", "COUNT", "STATUS" };

    read_tcp_connections("/proc/net/tcp",  &conns4, &count4);
    read_tcp_connections("/proc/net/tcp6", &conns6, &count6);

    total = count4 + count6;
    if (total == 0) {
        printf("No TCP connections found.\n");
        free(conns4);
        free(conns6);
        return 0;
    }

    for (i = 0; i < count4; i++) {
        int s = conns4[i].state;
        if (s >= 0 && s <= 11) state_counts[s]++;
    }
    for (i = 0; i < count6; i++) {
        int s = conns6[i].state;
        if (s >= 0 && s <= 11) state_counts[s]++;
    }

    printf("Total TCP connections (IPv4+IPv6): %d\n\n", total);
    print_table_header(hdr, 3);

    for (i = 1; i <= 11; i++) {
        if (state_counts[i] == 0) continue;
        char cnt_s[16];
        const char *row[3];
        snprintf(cnt_s, sizeof(cnt_s), "%d", state_counts[i]);
        row[0] = tcp_state_name(i);
        row[1] = cnt_s;
        row[2] = check_tcp_anomaly(i, state_counts[i]) ? "WARN" : "OK";
        print_table_row(row, 3);
    }

    free(conns4);
    free(conns6);
    return 0;
}
