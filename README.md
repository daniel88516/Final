# BusyBox 系統診斷工具集

## 專案簡介

在 BusyBox 中整合三個自製診斷 applet：

| Applet      | 功能                               |
| ----------- | -------------------------------- |
| `bbtop`     | 行程資源分析，顯示 PID、PPID、名稱、RSS        |
| `bbfscheck` | 檔案系統健康檢測，顯示容量、使用率，超過 80% 標記 WARN |
| `bbnetmon`  | TCP 連線狀態監控，依 state 統計，異常時標記 WARN |

三個 applet 共用 `libdiag` 模組庫，直接讀取 Linux `/proc` 介面。

---

## 系統架構

```
┌─────────────────────────────────────────────────┐
│          BusyBox 單一執行檔 / Applet Dispatch     │
├───────────────┬─────────────────┬───────────────┤
│    bbtop      │   bbfscheck     │   bbnetmon    │
│  行程資源分析  │ 檔案系統健康檢測 │  TCP 連線監控 │
└───────┬───────┴────────┬────────┴───────┬───────┘
        │                │                │
        ▼                ▼                ▼
┌─────────────────────────────────────────────────┐
│              libdiag 模組庫 (src/libdiag/)        │
│  proc_reader  │  fs_reader  │  net_reader        │
│  formatter    │  diag_common.h                   │
└──────────────────────┬──────────────────────────┘
                       ▼
┌─────────────────────────────────────────────────┐
│           Linux Kernel Interface                 │
│  /proc/[pid]/stat    statvfs()    /proc/net/tcp  │
│  /proc/[pid]/status  opendir()    /proc/net/tcp6 │
│  /proc/stat          readdir()                   │
└─────────────────────────────────────────────────┘
```

---

## 專案結構

```
Final/
├── src/
│   ├── applets/          # bbtop.c, bbfscheck.c, bbnetmon.c
│   └── libdiag/          # 拆分模組：proc/fs/net/formatter
├── busybox-meta/         # Config.src, Kbuild.src（整合 BusyBox 用）
├── third_party/busybox/  # BusyBox source（git submodule）
├── scripts/              # integrate / build / test / benchmark
├── tests/
│   ├── integration/      # 各 applet 整合測試
│   └── smoke/            # 環境前置檢查
└── Makefile
```

---

## Clone 與初始化

```bash
git clone https://github.com/daniel88516/Final.git
cd Final
git submodule update --init --recursive
```

---

## 工作流程

```bash
make init-submodule   # 初始化 BusyBox submodule（第一次 clone 後執行）
make integrate        # 把 src/ 的程式碼複製進 BusyBox 的 diagnostics/
make build            # 編譯 BusyBox（包含我們的 applets）
make test             # 執行 smoke + integration 測試
make bench            # 跑 benchmark
```

---

## 目前進度

### 完成項目

- [x] **專案結構重整**（2026-05-15）
  
  - BusyBox source 改為 git submodule（`third_party/busybox`）
  - 自己的程式碼與 BusyBox 完全分離

- [x] **libdiag 模組拆分與實作**（2026-05-15）
  
  - 原本的 `libdiag.c` 拆成四個獨立模組：
    - `proc_reader.c/h`：讀取 `/proc/[pid]/stat`、`status`、`/proc/stat`；實作 `diag_print_process_summary()`
    - `fs_reader.c/h`：`statvfs()`、遞迴 `scan_dir()`；實作 `diag_print_filesystems()`（讀 `/proc/mounts`，自動略過虛擬 FS）
    - `net_reader.c/h`：解析 `/proc/net/tcp` + `tcp6`；實作 `diag_print_tcp_summary()`（依 state 分組統計）
    - `formatter.c/h`：表格輸出、`check_fs_health()`、`check_tcp_anomaly()`

- [x] **Applet 實作**（bbtop / bbfscheck / bbnetmon）
  
  - 各 applet 呼叫對應 `diag_print_*()` 函式
  - `//applet:` 與 `//kbuild:` 已正確標記

- [x] **BusyBox 整合設定**
  
  - `busybox-meta/Config.src`：menuconfig 選項（BBTOP / BBFSCHECK / BBNETMON）
  - `busybox-meta/Kbuild.src`：build rules

- [x] **測試與腳本**
  
  - `tests/smoke/test_runtime_env.sh`：確認 `/proc` 路徑可存取
  - `tests/integration/`：各 applet 功能測試
  - `scripts/integrate_busybox.sh`：自動整合腳本

- [x] **文件**（2026-05-15）
  
  - `CONTRIBUTING.md`：環境需求、建置流程、測試、分支規範
  - `man/man1/bbtop.1`：bbtop man page
  - `man/man1/bbfscheck.1`：bbfscheck man page
  - `man/man1/bbnetmon.1`：bbnetmon man page

### 待完成

#### 組員 A
- [ ] `bbtop` 核心功能：CPU% 計算（兩次 `/proc/stat` 採樣 delta）、依 CPU/Memory 排序、PID tree 顯示
- [ ] `bbtop` CLI 選項：`--snapshot`、`--interval N`、`--sort FIELD`、`--tree`

#### 組員 B
- [ ] `bbfscheck` 核心功能：inode 使用率顯示、`--scan DIR` 目錄掃描模式
- [ ] `bbfscheck` CLI 選項：`--summary`、`--inode`、`--scan DIR`、`--max-depth N`、`--warn PERCENT`

#### 組員 C
- [ ] `bbnetmon` 核心功能：`--list` 列出所有連線的 local/remote address:port、state 篩選
- [ ] `bbnetmon` CLI 選項：`--summary`、`--list`、`--state STATE`

#### 組員 D
- [ ] 在 Docker 環境內執行 `make integrate && make build` 驗證編譯（等 A/B/C 核心功能完成後整合）
- [ ] `tests/integration/` 補充實際驗證邏輯（等各 applet 功能穩定後補上）
- [ ] Benchmark：與 `top`/`df`/`du`/`ss` 比較時間與記憶體用量（等編譯通過後執行）
- [ ] man page OPTIONS 欄位補齊（等 A/B/C CLI 選項確定後更新）

#### 全組
- [ ] 書面報告與簡報（5/24–5/26）
