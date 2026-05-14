#!/bin/sh
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)

echo "Running smoke tests"
sh "$ROOT_DIR/tests/smoke/test_runtime_env.sh"

echo "Running integration test placeholders"
for test_script in "$ROOT_DIR"/tests/integration/*.sh; do
	[ -f "$test_script" ] || continue
	sh "$test_script"
done

echo "Tests completed"
