# BusyBox 系統診斷工具集

## BusyBox 來源

本專案使用 BusyBox 官方 source code，透過 git submodule 方式引入至 `busybox/` 資料夾。

- 官方網站：https://busybox.net/
- Source code mirror：https://github.com/mirror/busybox

## Clone 專案

```bash
git clone --recurse-submodules https://github.com/daniel88516/Final.git
```

如果已經 clone 但沒有帶入 BusyBox source：

```bash
git submodule update --init --recursive
```
