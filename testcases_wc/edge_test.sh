#!/bin/bash
cd "$(dirname "$0")"

echo '===== Edge Cases 格式比對 ====='

check() {
    local desc="$1"
    local expected="$2"
    local actual="$3"
    if [ "$expected" = "$actual" ]; then
        echo "PASS: $desc"
    else
        echo "FAIL: $desc"
        echo "  Expected: $expected"
        echo "  Actual:   $actual"
    fi
}

# 空檔案
check "empty file" "$(wc empty.txt)" "$(../wc empty.txt)"

# 無結尾換行
printf 'hello world' > no_newline.txt
check "no newline" "$(wc no_newline.txt)" "$(../wc no_newline.txt)"

# 只有換行
printf '\n\n\n' > only_newlines.txt
check "only newlines" "$(wc only_newlines.txt)" "$(../wc only_newlines.txt)"

# 只有空白
printf '     ' > spaces_only.txt
check "spaces only" "$(wc spaces_only.txt)" "$(../wc spaces_only.txt)"

# 多個連續空白
printf 'a    b     c\n' > multi_space.txt
check "multi space" "$(wc multi_space.txt)" "$(../wc multi_space.txt)"

# 單一換行
printf '\n' > single_newline.txt
check "single newline" "$(wc single_newline.txt)" "$(../wc single_newline.txt)"

# 二進位檔案
check "binary" "$(wc -c binary.bin)" "$(../wc -c binary.bin)"
