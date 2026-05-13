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
