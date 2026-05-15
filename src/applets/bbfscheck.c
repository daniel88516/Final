//applet:IF_BBFSCHECK(APPLET(bbfscheck, BB_DIR_USR_BIN, BB_SUID_DROP))
//kbuild:lib-$(CONFIG_BBFSCHECK) += bbfscheck.o fs_reader.o formatter.o

//usage:#define bbfscheck_trivial_usage ""
//usage:#define bbfscheck_full_usage "\n\nDisplay filesystem usage and health summary (SIZE, USED, AVAIL, USE%, STATUS)"

#include "fs_reader.h"

#ifndef MAIN_EXTERNALLY_VISIBLE
#define MAIN_EXTERNALLY_VISIBLE
#endif

int bbfscheck_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int bbfscheck_main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    return diag_print_filesystems();
}
