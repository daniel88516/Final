#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include "mybox.h"

int do_ls(int argc, char **argv) {
    int show_all = 0;
    int opt;
    
    optind = 1;
    while ((opt = getopt(argc, argv, "a")) != -1) {
        switch (opt) {
            case 'a':
                show_all = 1;
                break;
            default:
                fprintf(stderr, "Usage: ls [-a] [directory]\n");
                return 1;
        }
    }
    
    const char *path = (optind < argc) ? argv[optind] : ".";
    
    DIR *dir = opendir(path);
    if (!dir) {
        perror("ls");
        return 1;
    }
    
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.')
            continue;
        printf("%s\n", entry->d_name);
    }
    
    closedir(dir);
    return 0;
}
