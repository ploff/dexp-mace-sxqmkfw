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

#pragma once

/* drive unselected matrix rows high (not floating) so a pressed key cannot
 * pull the shared LED column low and keep its PNP driver open */
#define MATRIX_UNSELECT_DRIVE_HIGH

/* The LEDs are driven as a 6 x 17 matrix.  The column lines are shared with
 * the key matrix (C12..C0, C15, B15, B13, B12); the RGB channels are the
 * eighteen pins on port A/B.  This is the SONiX shared-matrix topology used
 * by the OEM firmware, but with the roles of the C/D and A/B ports swapped
 * compared with the Keychron K8. */
#define SN32F2XX_RGB_MATRIX_ROW_PINS \
    { A8, A9, A10, A11, A12, A13, A14, A15, B0, B1, B2, B3, B4, B7, B5, D3, B14, B10 }

#define SN32F2XX_RGB_MATRIX_ROWS 6
#define SN32F2XX_RGB_MATRIX_COLS 17

/* PWM on the RGB row pins, GPIO on the shared column pins. */
/* Direction follows DIODE_DIRECTION (COL2ROW): PWM on the shared columns. */
#define SN32F2XX_PWM_CONTROL SOFTWARE_PWM

/* Active levels.  If a whole row is dark or the colours look inverted,
 * flip these two defines first. */
#define SN32F2XX_PWM_OUTPUT_ACTIVE_LEVEL SN32F2XX_PWM_OUTPUT_ACTIVE_LOW
#define SN32F2XX_RGB_OUTPUT_ACTIVE_LEVEL SN32F2XX_RGB_OUTPUT_ACTIVE_HIGH

#define RGB_MATRIX_LED_COUNT 87

/* Needed by the key-reactive and framebuffer effects. */
#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS

#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 255
#define RGB_MATRIX_DEFAULT_VAL 255
#define RGB_MATRIX_DEFAULT_SPD 100
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_MACE_STATIC
#define RGB_MATRIX_TYPING_HEATMAP_DECREASE_DELAY_MS 50



