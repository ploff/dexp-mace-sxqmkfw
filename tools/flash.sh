#!/usr/bin/env bash
# Flash the DEXP Mace firmware through the Sonix bootloader (0c45:7040).
set -euo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
FW="${1:-$HERE/firmware/dexp_mace_default.bin}"
SONIX="${SONIX:-sonixflasher}"

if [[ ! -f "$FW" ]]; then
    echo "Firmware not found: $FW" >&2
    exit 1
fi

if ! lsusb | grep -qi '0c45:7040'; then
    echo "Bootloader 0c45:7040 not found." >&2
    echo "Enter it with Fn+Esc, or short pin 3 to GND before plugging USB." >&2
    exit 1
fi

echo "Flashing: $FW"
exec "$SONIX" -v 0c45/7040 -f "$FW"
