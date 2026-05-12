#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "mybox.h"

int do_cp(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: cp <source> <destination>\n");
        return 1;
    }
    
    const char *src = argv[1];
    const char *dst = argv[2];
    
    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0) {
        perror("cp: source");
        return 1;
    }
    
    int dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd < 0) {
        perror("cp: destination");
        close(src_fd);
        return 1;
    }
    
    char buf[4096];
    ssize_t n;
    while ((n = read(src_fd, buf, sizeof(buf))) > 0) {
        if (write(dst_fd, buf, n) != n) {
            perror("cp: write");
            close(src_fd);
            close(dst_fd);
            return 1;
        }
    }
    
    close(src_fd);
    close(dst_fd);
    return 0;
}
