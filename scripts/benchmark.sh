#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
RESULT_DIR="$ROOT_DIR/benchmarks/results/local-$(date +%Y%m%d-%H%M%S)"

mkdir -p "$RESULT_DIR"

{
	echo "Diagnostics benchmark placeholder"
	echo "date: $(date -u)"
	echo "busybox: $ROOT_DIR/third_party/busybox/busybox"
} > "$RESULT_DIR/summary.txt"

echo "Benchmark placeholder results written to $RESULT_DIR/summary.txt"
