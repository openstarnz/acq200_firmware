#!/bin/bash
# run.sh [target...]
#
# Builds the acq32/acq200 kernel modules against several kernels in Docker
# containers and prints one PASS/FAIL line per target. Default: all targets.
# Each target's full build log is written to out/<target>.log.
# Exit status is nonzero if any target fails.
#
# The driver tree is mounted read-only; the build runs in a copy inside the
# container, so the worktree is never modified.
#
# Targets:
#   el6          CentOS 6, 2.6.32-754, x86_64 (production kernel)
#   deb12-amd64  Debian 12, 6.1, x86_64
#   deb12-i386   Debian 12, 6.1, 686 (32-bit)
#   deb13-amd64  Debian 13, 6.12, x86_64
#   pve614       Proxmox VE 6.14 headers on Debian 13, x86_64 (optional)

here=$(cd "$(dirname "$0")" && pwd)
src=$(cd "$here/../../.." && pwd)    # project/BASE/DTACQ/SRC
out="$here/out"
mkdir -p "$out"

# name | platform | dockerfile | build args
spec() {
    case "$1" in
    el6)         echo "linux/amd64|Dockerfile.el6|" ;;
    deb12-amd64) echo "linux/amd64|Dockerfile.debian|BASE=debian:12 HEADERS=linux-headers-amd64" ;;
    deb12-i386)  echo "linux/386|Dockerfile.debian|BASE=debian:12 HEADERS=linux-headers-686" ;;
    deb13-amd64) echo "linux/amd64|Dockerfile.debian|BASE=debian:trixie HEADERS=linux-headers-amd64" ;;
    pve614)      echo "linux/amd64|Dockerfile.debian|BASE=debian:trixie PVE=trixie HEADERS=proxmox-headers-6.14" ;;
    *) return 1 ;;
    esac
}

targets=("$@")
[ ${#targets[@]} -gt 0 ] || targets=(el6 deb12-amd64 deb12-i386 deb13-amd64 pve614)

status=0
for t in "${targets[@]}"; do
    if ! s=$(spec "$t"); then
        echo "UNKNOWN $t"; status=1; continue
    fi
    IFS='|' read -r platform dockerfile bargs <<< "$s"
    log="$out/$t.log"
    img="acq32-kbuild-$t"

    buildargs=()
    for a in $bargs; do buildargs+=(--build-arg "$a"); done

    : > "$log"
    echo "=== image build ($t) ===" >> "$log"
    if ! docker build --platform "$platform" -f "$here/$dockerfile" \
            "${buildargs[@]}" -t "$img" "$here" >> "$log" 2>&1; then
        echo "FAIL $t: image build failed (see $log)"
        status=1; continue
    fi

    echo "=== module build ($t) ===" >> "$log"
    docker run --rm --platform "$platform" -v "$src":/src:ro "$img" "$t" >> "$log" 2>&1
    rc=$?
    errs=$(grep -cE '(^|[^a-z])error:' "$log")
    warns=$(grep -c 'warning:' "$log")
    rel=$(sed -n 's/^release: //p' "$log" | head -1)
    if [ $rc -eq 0 ]; then
        echo "PASS $t ($rel): errors=$errs warnings=$warns"
    else
        echo "FAIL $t ($rel): errors=$errs warnings=$warns (see $log)"
        status=1
    fi
done
exit $status
