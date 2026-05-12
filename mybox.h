#ifndef MYBOX_H
#define MYBOX_H

struct command {
    const char *name;
    int (*func)(int, char **);
};

int do_ls(int argc, char **argv);
int do_cp(int argc, char **argv);
int do_wc(int argc, char **argv); //新增wc函數

#endif
