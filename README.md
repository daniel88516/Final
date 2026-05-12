# mybox — BusyBox 系統診斷工具集

基於 BusyBox applet 架構的輕量化 Linux 系統監控與健康檢測工具。

## 專案結構

```
Final/
├── main.c          # applet dispatcher
├── mybox.h         # 共用標頭與函式宣告
├── Makefile        # 編譯與測試
├── bbtop.c         # 行程資源分析（組員 A）
├── bbfscheck.c     # 檔案系統健康檢測（組員 B）
├── bbnetmon.c      # TCP 連線監控（組員 C）
├── testcases/      # 本專題功能測試腳本
└── testcases_wc/   # 舊測資（HW1 wc 專用，不需理會）
```

## 環境需求

需要在 Linux 環境下執行（WSL、Docker 或原生 Linux 均可），需安裝 `gcc` 與 `make`。

### 使用 BusyDocker 統一開發環境

**建立並啟動容器**

```bash
cd C:\Users\danie\Desktop\Linuxwork\BusyDocker
wsl sudo ./build.sh --run
# 然後再輸入密碼
```

**進入容器**

```bash
docker exec -it busydocker_inst bash
cd /workspace/Final
```

## 編譯

```bash
make
```

## 執行方式

編譯後會產生 `mybox` 執行檔，並建立 symlink。

### 透過 symlink 執行

```bash
make install   # 建立 bbtop、bbfscheck、bbnetmon symlink
./bbtop
./bbfscheck
./bbnetmon
```

### 直接透過 mybox 執行

```bash
./mybox bbtop
./mybox bbfscheck
./mybox bbnetmon
```

## 測試

### 單一功能測試

```bash
bash testcases/test_bbtop.sh
bash testcases/test_bbfscheck.sh
bash testcases/test_bbnetmon.sh
```

### 全部測試

```bash
make test_all
```

## 清除編譯產物

```bash
make clean
```

## 分工

| 組員   | 負責項目                                |
| ---- | ----------------------------------- |
| 組員 A | `bbtop.c` — 行程資源分析                  |
| 組員 B | `bbfscheck.c` — 檔案系統健康檢測            |
| 組員 C | `bbnetmon.c` — TCP 連線監控             |
| 組員 D | `main.c`、`mybox.h`、`Makefile`、測試、文件 |
