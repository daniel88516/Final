//applet:IF_BBNETMON(APPLET(bbnetmon, BB_DIR_USR_BIN, BB_SUID_DROP))
//kbuild:lib-$(CONFIG_BBNETMON) += bbnetmon.o net_reader.o formatter.o

#include "net_reader.h"

#ifndef MAIN_EXTERNALLY_VISIBLE
#define MAIN_EXTERNALLY_VISIBLE
#endif

int bbnetmon_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int bbnetmon_main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    return diag_print_tcp_summary();
}
