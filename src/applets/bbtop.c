//applet:IF_BBTOP(APPLET(bbtop, BB_DIR_USR_BIN, BB_SUID_DROP))
//kbuild:lib-$(CONFIG_BBTOP) += bbtop.o proc_reader.o formatter.o

//usage:#define bbtop_trivial_usage ""
//usage:#define bbtop_full_usage "\n\nDisplay process resource summary (PID, PPID, NAME, CPU%, RSS)"

#include "proc_reader.h"

#ifndef MAIN_EXTERNALLY_VISIBLE
#define MAIN_EXTERNALLY_VISIBLE
#endif

int bbtop_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int bbtop_main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    return diag_print_process_summary();
}
