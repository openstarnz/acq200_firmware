#!/bin/sh
# Runs inside the container. /src is the read-only SRC tree
# (project/BASE/DTACQ/SRC); the build happens in a copy so the worktree
# stays clean. The full log goes to stdout; the last line is the verdict.
TARGET=${1:-unknown}

KDIR=$(ls -d /lib/modules/*/build /usr/src/kernels/* 2>/dev/null | head -1)
[ -n "$KDIR" ] || { echo "RESULT: no kernel headers found"; exit 2; }
KDIR=$(readlink -f "$KDIR")

echo "target:  $TARGET"
echo "kdir:    $KDIR"
echo "release: $(cat "$KDIR/include/config/kernel.release" 2>/dev/null)"
echo "machine: $(uname -m)"
gcc --version | head -1

cp -a /src /build || exit 2
cd /build/dt100/Linux/Drivers || exit 2

make -k modules KDIR="$KDIR" 2>&1
rc=$?

missing=""
for ko in acq32-drv.ko acq200-drv.ko; do
    [ -f "$ko" ] || missing="$missing $ko"
done
echo "RESULT: make rc=$rc missing=[${missing# }]"
[ "$rc" -eq 0 ] && [ -z "$missing" ]
