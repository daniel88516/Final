#!/bin/bash
cd "$(dirname "$0")"

echo '===== Error Handling 測試 ====='

# 1. 不存在的檔案
echo '--- 不存在的檔案 ---'
echo "系統 wc:"; wc ghost.txt 2>&1; echo "exit code: $?"
echo "你的 wc:"; ../wc ghost.txt 2>&1; echo "exit code: $?"

# 2. 所有檔案都不存在
echo '--- 所有檔案都不存在 ---'
echo "系統 wc:"; wc aaa.txt bbb.txt 2>&1; echo "exit code: $?"
echo "你的 wc:"; ../wc aaa.txt bbb.txt 2>&1; echo "exit code: $?"

# 3. 部分檔案不存在
echo '--- 部分檔案不存在 ---'
echo "系統 wc:"; wc simple.txt ghost.txt ten_lines.txt 2>&1; echo "exit code: $?"
echo "你的 wc:"; ../wc simple.txt ghost.txt ten_lines.txt 2>&1; echo "exit code: $?"

# 4. 無讀取權限
echo '--- 無讀取權限 ---'
touch noperm.txt && chmod 000 noperm.txt
echo "系統 wc:"; wc noperm.txt 2>&1; echo "exit code: $?"
echo "你的 wc:"; ../wc noperm.txt 2>&1; echo "exit code: $?"
chmod 644 noperm.txt

# 5. 無效選項
echo '--- 無效選項 ---'
echo "系統 wc:"; wc -x simple.txt 2>&1; echo "exit code: $?"
echo "你的 wc:"; ../wc -x simple.txt 2>&1; echo "exit code: $?"
