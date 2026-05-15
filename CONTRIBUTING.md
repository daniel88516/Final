# Contributing

## 環境需求

- Linux 環境（建議使用 Docker，參考 `進入Busydocker指令.txt`）
- `gcc`、`make`、`git`

## 第一次設定

```bash
git clone https://github.com/daniel88516/Final.git
cd Final
git submodule update --init --recursive
```

## 建置流程

```bash
make integrate   # 把 src/ 的程式碼複製進 BusyBox
make build       # 編譯 BusyBox（含三個診斷 applet）
```

編譯完成後執行檔在 `third_party/busybox/busybox`。

## 執行工具

```bash
./third_party/busybox/busybox bbtop
./third_party/busybox/busybox bbfscheck
./third_party/busybox/busybox bbnetmon
```

## 跑測試

```bash
make test
```

包含兩個階段：
1. **smoke test** — 確認 `/proc` 路徑可存取（`tests/smoke/`）
2. **integration test** — 各 applet 功能測試（`tests/integration/`）

## 專案結構

```
src/
├── applets/       # bbtop.c, bbfscheck.c, bbnetmon.c
└── libdiag/       # proc_reader, fs_reader, net_reader, formatter

busybox-meta/      # Config.src, Kbuild.src（BusyBox 整合用）
scripts/           # integrate / build / test / benchmark 腳本
tests/
├── integration/   # 各 applet 整合測試
└── smoke/         # 環境前置檢查
```

## 新增或修改 applet

1. 在 `src/applets/` 修改對應的 `.c` 檔
2. 若需要新的 libdiag 函式，在 `src/libdiag/` 對應模組新增
3. 跑 `make integrate` 同步到 BusyBox
4. 跑 `make build` 驗證編譯
5. 跑 `make test` 確認測試通過

## 分支規範

- `master` — 主線，保持可編譯
- `feature/<名稱>` — 新功能開發
- PR 前先確認 `make build` 與 `make test` 通過
