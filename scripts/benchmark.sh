#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUSYBOX="$ROOT_DIR/third_party/busybox/busybox"

if [ ! -x "$BUSYBOX" ]; then
	echo "error: busybox binary not found at $BUSYBOX"
	echo "hint:  run: make build"
	exit 1
fi

RESULT_DIR="$ROOT_DIR/benchmarks/results/local-$(date +%Y%m%d-%H%M%S)"
mkdir -p "$RESULT_DIR"

echo "Benchmarking bbtop..."
time "$BUSYBOX" bbtop > "$RESULT_DIR/bbtop.txt" 2>&1 || true

echo "Benchmarking bbfscheck..."
time "$BUSYBOX" bbfscheck > "$RESULT_DIR/bbfscheck.txt" 2>&1 || true

echo "Benchmarking bbnetmon..."
time "$BUSYBOX" bbnetmon > "$RESULT_DIR/bbnetmon.txt" 2>&1 || true

echo "Results saved to $RESULT_DIR"
