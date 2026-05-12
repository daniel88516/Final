#!/bin/bash

# Test script for wc command
# This script tests various scenarios for the wc implementation

echo "===== Testing wc command ====="
echo ""

MYBOX="../wc"

# Function to test wc and compare with system wc
test_wc() {
    local desc="$1"
    local opts="$2"
    shift 2
    local files=("$@")
    
    echo "Test: $desc"
    echo "Command: wc $opts ${files[*]}"
    echo "Expected:"
    wc $opts "${files[@]}"
    echo "Your output:"
    $MYBOX $opts "${files[@]}"
    echo ""
}

# Check if wc binary exists
if [ ! -f "$MYBOX" ]; then
    echo "Error: $MYBOX not found. Please build the project first."
    exit 1
fi

cd "$(dirname "$0")"

# Test 1: Simple text file
test_wc "Simple text file" "" "simple.txt"

# Test 2: Count lines only
test_wc "Count lines only" "-l" "simple.txt"

# Test 3: Count words only
test_wc "Count words only" "-w" "simple.txt"

# Test 4: Count bytes only
test_wc "Count bytes only" "-c" "simple.txt"

# Test 5: Multiple options
test_wc "Count lines and words" "-l -w" "simple.txt"

# Test 6: Empty file
test_wc "Empty file" "" "empty.txt"

# Test 7: File with exactly 10 lines
test_wc "Ten lines file" "-l" "ten_lines.txt"

# Test 8: File with tabs
test_wc "File with tabs" "" "tabs.txt"

# Test 9: Multiple files
test_wc "Multiple files" "" "simple.txt" "empty.txt" "ten_lines.txt"

# Test 10: Binary file
test_wc "Binary file" "-c" "binary.bin"

# Test 11: All options together
test_wc "All options together" "-l -w -c" "simple.txt"

echo "===== Test completed ====="
