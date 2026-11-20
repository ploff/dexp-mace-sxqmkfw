#!/usr/bin/env python3
"""DEXP Mace (RKGK890) final matrix.

Discovered by pin probing:
  rows: D11 D10 D9 D8 D7 D6
  cols: C12 C11 C10 C9 C8 C7 C6 C5 C4 C3 C2 C1 C0 C15 B15 B14 B13
"""
from pathlib import Path
import json
import sys

ROWS = ["D11", "D10", "D9", "D8", "D7", "D6"]
COLS = ["C12", "C11", "C10", "C9", "C8", "C7", "C6", "C5", "C4", "C3",
        "C2", "C1", "C0", "C15", "B15", "B13", "B12"]
NR, NC = len(ROWS), len(COLS)

BASE = [
    # 0   1    2    3    4    5    6    7    8    9    10   11   12   13      14     15     16
    ["KC_ESC", "KC_F1", "KC_F2", "KC_F3", "KC_F4", "KC_F5", "KC_F6", "KC_F7", "KC_F8", "KC_F9", "KC_F10", "KC_F11", "KC_F12", "KC_NO", "KC_PSCR", "KC_SCRL", "KC_PAUS"],
    ["KC_GRV", "KC_1", "KC_2", "KC_3", "KC_4", "KC_5", "KC_6", "KC_7", "KC_8", "KC_9", "KC_0", "KC_MINS", "KC_EQL", "KC_BSPC", "KC_INS", "KC_HOME", "KC_PGUP"],
    ["KC_TAB", "KC_Q", "KC_W", "KC_E", "KC_R", "KC_T", "KC_Y", "KC_U", "KC_I", "KC_O", "KC_P", "KC_LBRC", "KC_RBRC", "KC_BSLS", "KC_DEL", "KC_END", "KC_PGDN"],
    ["KC_CAPS", "KC_A", "KC_S", "KC_D", "KC_F", "KC_G", "KC_H", "KC_J", "KC_K", "KC_L", "KC_SCLN", "KC_QUOT", "KC_NO", "KC_ENT", "KC_NO", "KC_NO", "KC_NO"],
    ["KC_LSFT", "KC_NO", "KC_Z", "KC_X", "KC_C", "KC_V", "KC_B", "KC_N", "KC_M", "KC_COMM", "KC_DOT", "KC_SLSH", "KC_NO", "KC_RSFT", "KC_NO", "KC_UP", "KC_NO"],
    ["KC_LCTL", "KC_LGUI", "KC_LALT", "KC_NO", "KC_NO", "KC_NO", "KC_SPC", "KC_NO", "KC_NO", "KC_NO", "KC_RALT", "MO(1)", "KC_APP", "KC_RCTL", "KC_LEFT", "KC_DOWN", "KC_RGHT"],
]

FN0 = ["QK_BOOT", "KC_MPLY", "KC_VOLD", "KC_VOLU", "KC_MUTE", "KC_MSTP", "KC_MPRV", "KC_MNXT",
       "KC_MAIL", "KC_WHOM", "KC_CALC", "KC_WSCH", "KC_TRNS", "KC_TRNS", "KC_TRNS", "KC_TRNS", "KC_TRNS"]
FN = [FN0] + [["KC_TRNS"] * NC for _ in range(NR - 1)]


def fmt(row):
    return "        { " + ", ".join(row) + " },"


def write(root: Path):
    board = root / "keyboards" / "dexp" / "mace"
    km = board / "keymaps" / "default"
    km.mkdir(parents=True, exist_ok=True)

    board.joinpath("config.h").write_text("#pragma once\n")

    layout = []
    for r in range(NR):
        for c in range(NC):
            layout.append({"matrix": [r, c], "x": c, "y": r})

    board_json = {
        "keyboard_name": "DEXP Mace",
        "manufacturer": "DEXP",
        "maintainer": "mace-linux",
        "usb": {"vid": "0x1209", "pid": "0x0001", "device_version": "1.0.0"},
        "features": {
            "audio": False, "backlight": False, "bootmagic": False,
            "command": False, "console": False, "extrakey": True,
            "mousekey": False, "nkro": True, "rgb_matrix": False,
        },
        "debounce": 5,
        "development_board": "sn32f240b_shared_matrix",
        "diode_direction": "COL2ROW",
        "matrix_pins": {"cols": COLS, "rows": ROWS},
        "layouts": {"LAYOUT": {"layout": layout}},
    }
    board.joinpath("keyboard.json").write_text(json.dumps(board_json, indent=4) + "\n")

    lines = [
        "/* DEXP Mace (RKGK890) — matrix found by pin probing. */",
        "#include QMK_KEYBOARD_H",
        "",
        "enum layer_names { _BASE, _FN };",
        "",
        "const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {",
        "    [_BASE] = {",
    ]
    lines += [fmt(r) for r in BASE]
    lines += ["    },", "    [_FN] = {"]
    lines += [fmt(r) for r in FN]
    lines += ["    },", "};", ""]
    km.joinpath("keymap.c").write_text("\n".join(lines))
    print("written", board)
    print("rows:", ROWS)
    print("cols:", COLS)


if __name__ == "__main__":
    write(Path(sys.argv[1]) if len(sys.argv) > 1 else Path("/tmp/opencode/qmk_src"))
