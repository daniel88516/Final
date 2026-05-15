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

# Register diagnostics/Config.in in top-level Config.in if not already there
TOP_CONFIG="$BUSYBOX_DIR/Config.in"
if ! grep -q 'source diagnostics/Config.in' "$TOP_CONFIG"; then
	echo 'source diagnostics/Config.in' >> "$TOP_CONFIG"
	echo "Registered diagnostics/Config.in in $TOP_CONFIG"
fi

# Register diagnostics/ in BusyBox Makefile libs-y if not already there
TOP_MAKEFILE="$BUSYBOX_DIR/Makefile"
if ! grep -q 'diagnostics/' "$TOP_MAKEFILE"; then
	sed -i 's|util-linux/volume_id/ \\|util-linux/volume_id/ \\\n\t\tdiagnostics/ \\|' "$TOP_MAKEFILE"
	echo "Registered diagnostics/ in $TOP_MAKEFILE"
fi

echo "Done."
