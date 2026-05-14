#include "net_reader.h"

#include <stdio.h>
#include <string.h>

static int count_tcp_rows(const char *path, unsigned long *count)
{
    FILE *tcp = fopen(path, "r");
    char line[512];

    if (tcp == NULL) {
        return 0;
    }

    if (fgets(line, sizeof(line), tcp) == NULL) {
        fclose(tcp);
        return 0;
    }

    while (fgets(line, sizeof(line), tcp) != NULL) {
        if (strspn(line, " \t\r\n") != strlen(line)) {
            (*count)++;
        }
    }

    fclose(tcp);
    return 0;
}

int diag_print_tcp_summary(void)
{
    unsigned long tcp4 = 0;
    unsigned long tcp6 = 0;

    count_tcp_rows("/proc/net/tcp", &tcp4);
    count_tcp_rows("/proc/net/tcp6", &tcp6);

    printf("family connections\n");
    printf("tcp4 %lu\n", tcp4);
    printf("tcp6 %lu\n", tcp6);
    return 0;
}
