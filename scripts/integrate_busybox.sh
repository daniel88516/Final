#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUSYBOX_DIR="$ROOT_DIR/third_party/busybox"
DIAG_DIR="$BUSYBOX_DIR/diagnostics"

if [ ! -d "$BUSYBOX_DIR" ]; then
	echo "error: $BUSYBOX_DIR does not exist"
	echo "hint:  run: make init-submodule"
	exit 1
fi

echo "Integrating diagnostics applets into $DIAG_DIR"
mkdir -p "$DIAG_DIR"

cp "$ROOT_DIR"/src/applets/*.c      "$DIAG_DIR"/
cp "$ROOT_DIR"/src/libdiag/*.[ch]   "$DIAG_DIR"/
cp "$ROOT_DIR"/busybox-meta/Config.src "$DIAG_DIR"/Config.src
cp "$ROOT_DIR"/busybox-meta/Kbuild.src "$DIAG_DIR"/Kbuild.src

echo "Done. Wire diagnostics/Config.src into BusyBox configuration if not already included."
