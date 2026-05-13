# BusyBox 系統診斷工具集

## BusyBox 來源

本專案使用 BusyBox 官方 source code，直接放置於 `busybox/` 資料夾。

- 官方網站：https://busybox.net/
- Source code mirror：https://github.com/mirror/busybox

## Clone 專案

```bash
git clone https://github.com/daniel88516/Final.git
```

## 系統架構

```
┌─────────────────────────────────────────────────┐
│          BusyBox 單一執行檔 / Applet Dispatch     │
├───────────────┬─────────────────┬───────────────┤
│    bbtop      │   bbfscheck     │   bbnetmon    │
│  行程資源分析  │ 檔案系統健康檢測 │  TCP 連線監控 │
└───────┬───────┴────────┬────────┴───────┬───────┘
        │                │                │
        └────────────────┼────────────────┘
                         ▼
┌─────────────────────────────────────────────────┐
│      libdiag 共用模組                            │
│  proc_reader / fs_reader / net_reader            │
│  formatter / rule_checker                        │
└──────────────────┬──────────────────────────────┘
                   ▼
┌─────────────────────────────────────────────────┐
│           Linux Kernel Interface                 │
│  /proc/[pid]/stat    statvfs()    /proc/net/tcp  │
│  /proc/[pid]/status  opendir()    /proc/net/tcp6 │
│  /proc/stat          readdir()                   │
└─────────────────────────────────────────────────┘
```





## 目前進度

### 1. 加入 BusyBox source code

直接從官方 clone BusyBox source code 放入 `busybox/` 資料夾：

```
git clone https://git.busybox.net/busybox busybox
```

### 2. 修改 `busybox/Makefile`

在 `libs-y` 最後加入 `diagnostics/`，讓 build system 知道有這個資料夾：

```
libs-y += diagnostics/
```

### 3. 建立 `busybox/diagnostics/`

新增以下檔案：

- `bbtop.c`、`bbfscheck.c`、`bbnetmon.c` — 各工具的實作（含 `//applet:` 註冊）
- `libdiag.c`、`libdiag.h` — 共用模組
- `Kbuild.src` — 告訴 build system 要編譯哪些檔案
- `Config.src` — menuconfig 設定選項
