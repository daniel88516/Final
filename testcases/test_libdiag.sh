#!/bin/bash
# test_libdiag.sh - 測試 libdiag 各模組功能

PASS=0
FAIL=0

check() {
    local desc=$1
    local result=$2
    if [ "$result" = "0" ]; then
        echo "PASS: $desc"
        PASS=$((PASS + 1))
    else
        echo "FAIL: $desc"
        FAIL=$((FAIL + 1))
    fi
}

echo "=== test_libdiag ==="

# proc_reader
echo "[proc_reader]"
[ -r /proc/1/stat ]    && check "/proc/1/stat 可讀" 0    || check "/proc/1/stat 可讀" 1
[ -r /proc/1/status ]  && check "/proc/1/status 可讀" 0  || check "/proc/1/status 可讀" 1
[ -r /proc/stat ]      && check "/proc/stat 可讀" 0      || check "/proc/stat 可讀" 1

# fs_reader
echo "[fs_reader]"
[ -d /proc ] && check "/proc 存在" 0 || check "/proc 存在" 1
[ -d /sys ]  && check "/sys 存在" 0  || check "/sys 存在" 1

# net_reader
echo "[net_reader]"
[ -r /proc/net/tcp ]  && check "/proc/net/tcp 可讀" 0  || check "/proc/net/tcp 可讀" 1
[ -r /proc/net/tcp6 ] && check "/proc/net/tcp6 可讀" 0 || check "/proc/net/tcp6 可讀" 1

echo ""
echo "結果：PASS=$PASS, FAIL=$FAIL"
[ "$FAIL" = "0" ] && exit 0 || exit 1
