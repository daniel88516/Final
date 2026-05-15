#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUSYBOX_DIR="$ROOT_DIR/third_party/busybox"

if [ ! -d "$BUSYBOX_DIR" ]; then
	echo "error: $BUSYBOX_DIR does not exist"
	echo "hint:  run: make init-submodule"
	exit 1
fi

cd "$BUSYBOX_DIR"

if [ -f "$ROOT_DIR/configs/busybox_diag_defconfig" ]; then
	cp "$ROOT_DIR/configs/busybox_diag_defconfig" .config
	make olddefconfig
else
	make defconfig
fi

make

if [ -f busybox ]; then
	printf "BusyBox binary: %s\n" "$BUSYBOX_DIR/busybox"
	ls -lh busybox
fi
