#include "proc_reader.h"

#include <stdio.h>

int diag_print_process_summary(void)
{
    FILE *stat = fopen("/proc/stat", "r");
    char line[256];

    if (stat == NULL) {
        perror("bbtop: /proc/stat");
        return 1;
    }

    if (fgets(line, sizeof(line), stat) != NULL) {
        printf("%s", line);
    }

    fclose(stat);
    return 0;
}
