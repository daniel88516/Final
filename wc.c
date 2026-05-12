// Generated with AI assistance
#include <stdio.h>      // printf, fprintf, perror
#include <stdlib.h>     // calloc, free
#include <string.h>     // strerror()
#include <errno.h>      // errno
#include <fcntl.h>      // open() 函數
#include <unistd.h>     // read(), close(), getopt()
#include <sys/stat.h>   // stat() 函數
#include "mybox.h"      // 引入 mybox 的 header

// 計算數字位數的輔助函數，用來決定輸出的對齊寬度
// 例如：137 → 3位數，66 → 2位數
static int num_digits(long n) {
    if (n == 0) return 1; // 0 本身是 1 位數
    int d = 0;
    while (n > 0) { d++; n /= 10; } // 每次除以 10，計算位數
    return d;
}

// 計算單一檔案的行數、字數、bytes
// 回傳 0 成功，-1 失敗
static int count_file(const char *filename, long *lines, long *words, long *bytes) {
    *lines = *words = *bytes = 0; // 初始化計數器

    // 用 stat() 取得檔案資訊（包含 bytes 大小）
    struct stat st;
    if (stat(filename, &st) == -1) {
        return -1; // 錯誤由呼叫端負責印出
    }
    *bytes = st.st_size; // 直接從 stat 取得檔案大小

    // 用 open() 開啟檔案，O_RDONLY 代表唯讀
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        return -1; // 錯誤由呼叫端負責印出
    }

    int in_word = 0; // 記錄目前是否在一個單字內
    char buf[4096];  // 讀取緩衝區，每次最多讀 4096 bytes
    ssize_t n;       // 實際讀到的 bytes 數

    // 不斷讀取直到檔案結束
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        for (int j = 0; j < n; j++) {
            if (buf[j] == '\n') (*lines)++; // 遇到換行，行數 +1
            // 遇到空白、換行、tab 代表單字結束
            if (buf[j] == ' ' || buf[j] == '\n' || buf[j] == '\t') {
                in_word = 0;
            } else {
                // 遇到非空白且不在單字內，代表新單字開始
                if (!in_word) { (*words)++; in_word = 1; }
            }
        }
    }

    close(fd); // 關閉檔案
    return 0;
}

int do_wc(int argc, char **argv) {
    // 三個 flag 記錄要顯示哪些資訊
    int show_lines = 0, show_words = 0, show_bytes = 0;
    int opt;

    optind = 1; // 重設 getopt 起始位置
    // 解析命令列選項 -l -w -c
    while ((opt = getopt(argc, argv, "lwc")) != -1) {
        switch (opt) {
            case 'l': show_lines = 1; break; // -l 顯示行數
            case 'w': show_words = 1; break; // -w 顯示字數
            case 'c': show_bytes = 1; break; // -c 顯示 bytes
            default:
                fprintf(stderr, "Usage: wc [-l] [-w] [-c] [file...]\n");
                return 1;
        }
    }

    // 沒有指定選項時，預設三個全部顯示
    if (!show_lines && !show_words && !show_bytes)
        show_lines = show_words = show_bytes = 1;

    // 沒有提供檔案名稱時，印出使用說明
    if (optind >= argc) {
        fprintf(stderr, "Usage: wc [-l] [-w] [-c] [file...]\n");
        return 1;
    }

    int file_count = argc - optind; // 計算檔案數量

    // 動態分配陣列來儲存每個檔案的統計結果
    long *all_lines = calloc(file_count, sizeof(long)); // 每個檔案的行數
    long *all_words = calloc(file_count, sizeof(long)); // 每個檔案的字數
    long *all_bytes = calloc(file_count, sizeof(long)); // 每個檔案的 bytes
    int  *valid     = calloc(file_count, sizeof(int));  // 記錄哪些檔案開啟成功
    int  *errnos    = calloc(file_count, sizeof(int));  // 記錄失敗檔案的 errno

    if (!all_lines || !all_words || !all_bytes || !valid || !errnos) {
        fprintf(stderr, "wc: memory allocation failed\n");
        free(all_lines); free(all_words); free(all_bytes); free(valid); free(errnos);
        return 1;
    }

    long max_val = 0; // 記錄所有數字中的最大值，用來決定輸出寬度
    int had_error = 0;   // 記錄是否有任何檔案處理失敗
    int valid_count = 0; // 記錄成功處理的檔案數

    // 第一階段：先算出所有檔案的結果
    for (int i = 0; i < file_count; i++) {
        if (count_file(argv[optind + i], &all_lines[i], &all_words[i], &all_bytes[i]) == 0) {
            valid[i] = 1; // 標記此檔案成功
            valid_count++;
            // 更新最大值
            if (show_lines && all_lines[i] > max_val) max_val = all_lines[i];
            if (show_words && all_words[i] > max_val) max_val = all_words[i];
            if (show_bytes && all_bytes[i] > max_val) max_val = all_bytes[i];
        } else {
            had_error = 1;        // 標記有檔案失敗
            errnos[i] = errno;    // 儲存錯誤碼
        }
    }

    // 計算所有檔案的 total
    long total_lines = 0, total_words = 0, total_bytes = 0;
    for (int i = 0; i < file_count; i++) {
        if (valid[i]) {
            total_lines += all_lines[i];
            total_words += all_words[i];
            total_bytes += all_bytes[i];
        }
    }
    // total 也要納入最大值比較
    if (file_count > 1) {
        if (show_lines && total_lines > max_val) max_val = total_lines;
        if (show_words && total_words > max_val) max_val = total_words;
        if (show_bytes && total_bytes > max_val) max_val = total_bytes;
    }

    // 根據最大數字的位數決定輸出寬度
    int width = num_digits(max_val);

    // 第二階段：照順序印出結果，用 %*ld 動態指定寬度
    for (int i = 0; i < file_count; i++) {
        if (!valid[i]) {
            // 先刷新 stdout，確保錯誤訊息出現在正確位置
            fflush(stdout);
            fprintf(stderr, "wc: %s: %s\n", argv[optind + i], strerror(errnos[i]));
            continue;
        }
        if (show_lines) printf("%*ld ", width, all_lines[i]);
        if (show_words) printf("%*ld ", width, all_words[i]);
        if (show_bytes) printf("%*ld ", width, all_bytes[i]);
        printf("%s\n", argv[optind + i]); // 印出檔名
    }

    // 多個檔案時顯示 total
    if (file_count > 1) {
        if (show_lines) printf("%*ld ", width, total_lines);
        if (show_words) printf("%*ld ", width, total_words);
        if (show_bytes) printf("%*ld ", width, total_bytes);
        printf("total\n");
    }

    // 釋放動態分配的記憶體
    free(all_lines);
    free(all_words);
    free(all_bytes);
    free(valid);
    free(errnos);

    return had_error; // 有任何檔案失敗則回傳 1
}