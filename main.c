#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include "mybox.h"

static const struct command commands[] = {
    { "ls", do_ls },
    { "cp", do_cp },
    { "wc", do_wc }, // 新增 wc 指令
    { NULL, NULL }
};

int main(int argc, char **argv) {
    char *base = basename(argv[0]);
    
    for (int i = 0; commands[i].name; i++) {
        if (strcmp(base, commands[i].name) == 0) {
            return commands[i].func(argc, argv);
        }
    }
    
    if (argc > 1) {
        for (int i = 0; commands[i].name; i++) {
            if (strcmp(argv[1], commands[i].name) == 0) {
                return commands[i].func(argc - 1, argv + 1);
            }
        }
    }
    
    fprintf(stderr, "Usage: %s [command] [args]\n", argv[0]);
    fprintf(stderr, "Commands: ls, cp, wc\n");  //總共有ls, cp , wc 三種指令可以使用
    return 1;
}
