/* Copyright 2026 dexp-mace-qmkfw
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* DEXP Mace (RKGK890) — matrix found by pin probing.
 *
 * The Fn layer follows the original manual:
 *   Fn+Scroll Lock  — next lighting mode (19 modes)
 *   Fn+Home         — next preset colour
 *   Fn+Up / Fn+Down — brightness + / -
 *   Fn+Left / Right — effect speed - / +
 *   Fn+W            — swap WASD with the arrow keys
 *   Fn+Esc          — bootloader (the original key resets settings)
 */
#include QMK_KEYBOARD_H

enum mace_keycodes {
    MACE_COLOR_NEXT = SAFE_RANGE,
    MACE_WASD_SWAP,
};

enum layer_names { _BASE, _FN };

typedef union {
    uint32_t raw;
    struct {
        uint32_t wasd_swapped : 1;
    };
} user_config_t;

static user_config_t user_config;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = {
        { KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_NO, KC_PSCR, KC_SCRL, KC_PAUS },
        { KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_INS, KC_HOME, KC_PGUP },
        { KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL, KC_END, KC_PGDN },
        { KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NO, KC_ENT, KC_NO, KC_NO, KC_NO },
        { KC_LSFT, KC_NO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_NO, KC_RSFT, KC_NO, KC_UP, KC_NO },
        { KC_LCTL, KC_LGUI, KC_LALT, KC_NO, KC_NO, KC_NO, KC_SPC, KC_NO, KC_NO, KC_NO, KC_RALT, MO(1), KC_APP, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT },
    },
    [_FN] = {
        { QK_BOOT, KC_MYCM, KC_WHOM, KC_MAIL, KC_MSEL, KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP, KC_VOLU, KC_VOLD, KC_MUTE, KC_CALC, KC_TRNS, KC_TRNS, RM_NEXT, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, MACE_COLOR_NEXT, KC_TRNS },
        { KC_TRNS, KC_TRNS, MACE_WASD_SWAP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RM_VALU, KC_TRNS },
        { KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RM_SPDD, RM_VALD, RM_SPDU },
    },
};

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();

}

static void mace_next_color(void) {
    static const hsv_t colors[] = {
        {HSV_WHITE},
        {HSV_RED},
        {HSV_ORANGE},
        {HSV_YELLOW},
        {HSV_GREEN},
        {HSV_CYAN},
        {HSV_BLUE},
        {HSV_PURPLE},
    };

    static uint8_t color_index = 0;

    color_index = (color_index + 1) % ARRAY_SIZE(colors);
    rgb_matrix_sethsv(colors[color_index].h, colors[color_index].s, colors[color_index].v);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == MACE_COLOR_NEXT) {
        if (record->event.pressed) {
            mace_next_color();
        }
        return false;
    }

    if (keycode == MACE_WASD_SWAP) {
        if (record->event.pressed) {
            user_config.wasd_swapped = !user_config.wasd_swapped;
            eeconfig_update_user(user_config.raw);
        }
        return false;
    }

    if (user_config.wasd_swapped) {
        uint16_t swapped = KC_NO;

        switch (keycode) {
            case KC_W:
                swapped = KC_UP;
                break;
            case KC_A:
                swapped = KC_LEFT;
                break;
            case KC_S:
                swapped = KC_DOWN;
                break;
            case KC_D:
                swapped = KC_RGHT;
                break;
            case KC_UP:
                swapped = KC_W;
                break;
            case KC_LEFT:
                swapped = KC_A;
                break;
            case KC_DOWN:
                swapped = KC_S;
                break;
            case KC_RGHT:
                swapped = KC_D;
                break;
            default:
                break;
        }

        if (swapped != KC_NO) {
            if (record->event.pressed) {
                register_code16(swapped);
            } else {
                unregister_code16(swapped);
            }
            return false;
        }
    }

    return true;
}
