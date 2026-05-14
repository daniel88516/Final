#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUSYBOX_DIR="$ROOT_DIR/third_party/busybox"
DIAG_DIR="$BUSYBOX_DIR/diagnostics"

if [ ! -d "$BUSYBOX_DIR" ]; then
	echo "error: $BUSYBOX_DIR does not exist"
	echo "hint: add or initialize the BusyBox submodule first:"
	echo "      git submodule add https://github.com/mirror/busybox.git third_party/busybox"
	echo "      git submodule update --init --recursive"
	exit 1
fi

echo "Integrating diagnostics applets into $DIAG_DIR"
mkdir -p "$DIAG_DIR"

cp "$ROOT_DIR"/src/applets/*.c "$DIAG_DIR"/
cp "$ROOT_DIR"/src/libdiag/*.[ch] "$DIAG_DIR"/
cp "$ROOT_DIR"/busybox-meta/Config.src "$DIAG_DIR"/Config.src
cp "$ROOT_DIR"/busybox-meta/Kbuild.src "$DIAG_DIR"/Kbuild.src

echo "Done. BusyBox diagnostics sources were refreshed."
echo "Note: wire diagnostics/Config.src into BusyBox configuration if it is not already included."
