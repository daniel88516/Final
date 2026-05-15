#!/bin/sh
set -eu

missing=0

for path in /proc/stat /proc/1/stat /proc/1/status /proc/net/tcp /proc/net/tcp6; do
	if [ ! -r "$path" ]; then
		echo "missing or unreadable: $path"
		missing=1
	else
		echo "ok: $path"
	fi
done

exit "$missing"
