#!/bin/bash
# pci-rom-dump.sh — dump PCI expansion ROM contents without loading a driver.
#
# Usage: pci-rom-dump.sh <bdf>              # info-only, no writes
#        pci-rom-dump.sh <bdf> read [N]     # enable ROM, dump N bytes
#                                           # (default 256), restore original
#                                           # PCI_ROM_ADDRESS on exit
#        pci-rom-dump.sh <bdf> read-at OFF LEN
#                                           # enable ROM, dump LEN bytes from
#                                           # offset OFF (hex or decimal),
#                                           # then restore.
#
# Reads/writes PCI config space via /sys/bus/pci/.../config and reads ROM
# contents via /dev/mem.  Needs root and CONFIG_STRICT_DEVMEM=n (which
# CentOS 6 default kernels satisfy).
#
# Restores the original PCI_ROM_ADDRESS on any exit path (trap EXIT), so a
# botched debug session doesn't leave the chip in "ROM enabled" state.
#
# Safe for use without our acq32 driver loaded — this is the whole point
# of the tool: read ROM directly, bypassing any driver-specific bugs.

set -eu

BDF="${1:-}"
MODE="${2:-info}"
[ -n "$BDF" ] || { echo "usage: $0 <bdf> [info|read [len]|read-at off len]" >&2; exit 1; }

D=/sys/bus/pci/devices/$BDF
[ -d "$D" ] || { echo "no such device: $D" >&2; exit 1; }

byte_at() {
    # $1 = byte offset in config space; prints two hex chars.
    od -An -tx1 -N 1 -j "$1" "$D/config" | tr -d ' '
}
u16le_at() {
    # $1 = byte offset; prints "0xHHHH" (little-endian 16-bit).
    local lo hi
    lo=$(byte_at "$1")
    hi=$(byte_at $(($1 + 1)))
    echo "0x${hi}${lo}"
}
u32le_at() {
    # $1 = byte offset; prints "0xHHHHHHHH" (little-endian 32-bit).
    local b0 b1 b2 b3
    b0=$(byte_at "$1")
    b1=$(byte_at $(($1 + 1)))
    b2=$(byte_at $(($1 + 2)))
    b3=$(byte_at $(($1 + 3)))
    echo "0x${b3}${b2}${b1}${b0}"
}
write_u32le_at() {
    # $1 = byte offset in config space, $2 = 32-bit value (decimal or 0xNN).
    local off val b0 b1 b2 b3
    off=$1
    val=$(($2))
    b0=$(printf '%02x' $((val         & 0xff)))
    b1=$(printf '%02x' $(((val >> 8)  & 0xff)))
    b2=$(printf '%02x' $(((val >> 16) & 0xff)))
    b3=$(printf '%02x' $(((val >> 24) & 0xff)))
    printf "\\x${b0}\\x${b1}\\x${b2}\\x${b3}" \
        | dd of="$D/config" bs=1 seek="$off" count=4 conv=notrunc 2>/dev/null
}

# ----- info -----
V=$(cat "$D/vendor")
P=$(cat "$D/device")
CMD=$(u16le_at 4)
ROM_ORIG=$(u32le_at 48)
echo "device: $BDF  vendor=$V  device=$P"
printf "COMMAND: %s  (Mem=%s IO=%s BusMaster=%s)\n" "$CMD" \
    $([ $(( CMD & 0x02 )) -ne 0 ] && echo Y || echo N) \
    $([ $(( CMD & 0x01 )) -ne 0 ] && echo Y || echo N) \
    $([ $(( CMD & 0x04 )) -ne 0 ] && echo Y || echo N)
printf "PCI_ROM_ADDRESS: %s  (ENABLE=%s)\n" "$ROM_ORIG" \
    $([ $(( ROM_ORIG & 0x01 )) -ne 0 ] && echo Y || echo N)

# 7th line of resource = PCI_ROM_RESOURCE
ROM_LINE=$(sed -n 7p "$D/resource")
ROM_START=$(echo "$ROM_LINE" | awk '{print $1}')
ROM_END=$(echo "$ROM_LINE"   | awk '{print $2}')
ROM_LEN=$(( $(($ROM_END)) - $(($ROM_START)) + 1 ))
printf "ROM resource: start=%s end=%s len=%d bytes\n" \
    "$ROM_START" "$ROM_END" "$ROM_LEN"

if [ $(($ROM_START)) -eq 0 ]; then
    echo "  -> kernel did not allocate a ROM BAR for this device"
    exit 0
fi

[ "$MODE" = "info" ] && exit 0

# ----- read modes -----
OFF=0
LEN=256
case "$MODE" in
    read)     LEN=${3:-256} ;;
    read-at)  OFF=${3:-0}; LEN=${4:-256} ;;
    *)        echo "unknown mode: $MODE" >&2; exit 1 ;;
esac
OFF_N=$(($OFF))
LEN_N=$(($LEN))

# Restore original PCI_ROM_ADDRESS on any exit.  Do NOT touch the COMMAND
# register — its bits may be needed by whatever driver is bound.
trap "write_u32le_at 48 $ROM_ORIG; echo 'PCI_ROM_ADDRESS restored to $ROM_ORIG'" EXIT

# Enable memory decode if not already (needed to read from ROM BAR)
if [ $(( CMD & 0x02 )) -eq 0 ]; then
    echo "enabling Memory Space bit in COMMAND"
    NEW_CMD=$((CMD | 0x02))
    b0=$(printf '%02x' $(( NEW_CMD       & 0xff )))
    b1=$(printf '%02x' $(( (NEW_CMD >> 8) & 0xff )))
    printf "\\x${b0}\\x${b1}" | dd of="$D/config" bs=1 seek=4 count=2 conv=notrunc 2>/dev/null
fi

# Set ENABLE bit on PCI_ROM_ADDRESS (preserve address bits)
ROM_ENABLED=$(( $(($ROM_ORIG)) | 0x01 ))
printf "writing PCI_ROM_ADDRESS = 0x%08x  (ENABLE bit set)\n" $ROM_ENABLED
write_u32le_at 48 $ROM_ENABLED

# Confirm the write took
ROM_READBACK=$(u32le_at 48)
printf "PCI_ROM_ADDRESS readback: %s  " "$ROM_READBACK"
if [ $(($ROM_READBACK)) -eq $ROM_ENABLED ]; then
    echo "OK"
else
    printf "*MISMATCH* — chip masked bits (expected 0x%08x)\n" $ROM_ENABLED
fi

# Dump ROM contents from /dev/mem
READ_AT=$(( $(($ROM_START)) + OFF_N ))
printf "dumping %d bytes from ROM offset 0x%x (physaddr 0x%x)\n" \
    $LEN_N $OFF_N $READ_AT
echo "--- hexdump ---"
dd if=/dev/mem bs=1 skip=$READ_AT count=$LEN_N 2>/dev/null | hexdump -C
echo "---------------"
# trap fires -> restores PCI_ROM_ADDRESS
