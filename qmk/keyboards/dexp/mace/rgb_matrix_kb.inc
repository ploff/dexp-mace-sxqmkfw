/* DEXP Mace (RKGK890) — original lighting modes
 *
 * The vendor firmware exposes nineteen effects through Fn+Scroll Lock.
 * They are reproduced here as RGB Matrix custom effects so that the mode
 * order and the Fn shortcuts match the original keyboard:
 *
 *   1  Static          8  Spectrum       15 Ripples
 *   2  SingleOn        9  Outward        16 Flowing
 *   3  SingleOff      10  Scrolling      17 Pulsating
 *   4  Glittering     11  Rolling        18 Tilt
 *   5  Falling        12  Rotating       19 Shuttle
 *   6  Colourful      13  Explode
 *   7  Breath         14  Launch
 *
 * No #pragma once here: QMK includes this file several times with different
 * definitions of RGB_MATRIX_EFFECT.
 */

RGB_MATRIX_EFFECT(MACE_STATIC)
RGB_MATRIX_EFFECT(MACE_SINGLE_ON)
RGB_MATRIX_EFFECT(MACE_SINGLE_OFF)
RGB_MATRIX_EFFECT(MACE_GLITTER)
RGB_MATRIX_EFFECT(MACE_FALLING)
RGB_MATRIX_EFFECT(MACE_COLOURFUL)
RGB_MATRIX_EFFECT(MACE_BREATH)
RGB_MATRIX_EFFECT(MACE_SPECTRUM)
RGB_MATRIX_EFFECT(MACE_OUTWARD)
RGB_MATRIX_EFFECT(MACE_SCROLLING)
RGB_MATRIX_EFFECT(MACE_ROLLING)
RGB_MATRIX_EFFECT(MACE_ROTATING)
RGB_MATRIX_EFFECT(MACE_EXPLODE)
RGB_MATRIX_EFFECT(MACE_LAUNCH)
RGB_MATRIX_EFFECT(MACE_RIPPLES)
RGB_MATRIX_EFFECT(MACE_FLOWING)
RGB_MATRIX_EFFECT(MACE_PULSATING)
RGB_MATRIX_EFFECT(MACE_TILT)
RGB_MATRIX_EFFECT(MACE_SHUTTLE)
RGB_MATRIX_EFFECT(MACE_OFF)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#    include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Common helpers                                                     */
/* ------------------------------------------------------------------ */

static inline uint8_t mace_fade(uint8_t value, uint8_t amount) {
    return value > amount ? value - amount : 0;
}

static inline uint8_t mace_wave(uint8_t phase, uint8_t value) {
    return scale8(sin8(phase), value);
}

/* A single random sparkle buffer, used by Glittering. */
static uint8_t mace_glitter_buf[RGB_MATRIX_LED_COUNT];

/* ------------------------------------------------------------------ */
/* 1. Static                                                          */
/* ------------------------------------------------------------------ */

static hsv_t mace_static_math(hsv_t hsv, uint8_t i, uint8_t time) {
    return hsv;
}

bool MACE_STATIC(effect_params_t* params) {
    return effect_runner_i(params, &mace_static_math);
}

/* ------------------------------------------------------------------ */
/* 2. SingleOn — pressed key lights up, then fades out                */
/* ------------------------------------------------------------------ */

static hsv_t mace_single_on_math(hsv_t hsv, uint16_t offset) {
    if (offset > 255) offset = 255;
    hsv.v = scale8(255 - offset, hsv.v);
    return hsv;
}

bool MACE_SINGLE_ON(effect_params_t* params) {
    return effect_runner_reactive(params, &mace_single_on_math);
}

/* ------------------------------------------------------------------ */
/* 3. SingleOff — pressed key goes dark, then fades back in           */
/* ------------------------------------------------------------------ */

static hsv_t mace_single_off_math(hsv_t hsv, uint16_t offset) {
    if (offset > 255) offset = 255;
    hsv.v = scale8(offset, hsv.v);
    return hsv;
}

bool MACE_SINGLE_OFF(effect_params_t* params) {
    return effect_runner_reactive(params, &mace_single_off_math);
}

/* ------------------------------------------------------------------ */
/* 4. Glittering                                                      */
/* ------------------------------------------------------------------ */

bool MACE_GLITTER(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (params->init) {
        memset(mace_glitter_buf, 0, sizeof(mace_glitter_buf));
    }

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        if ((rand() & 0x3F) == 0) {
            mace_glitter_buf[i] = 255;
        } else {
            mace_glitter_buf[i] = mace_fade(mace_glitter_buf[i], 10);
        }

        hsv_t hsv = rgb_matrix_config.hsv;
        hsv.v     = scale8(hsv.v, mace_glitter_buf[i]);
        rgb_t rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }

    return rgb_matrix_check_finished_leds(led_max);
}

/* ------------------------------------------------------------------ */
/* 5. Falling — coloured digital rain                                 */
/* ------------------------------------------------------------------ */

#    ifndef MACE_RAIN_DROPS
#        define MACE_RAIN_DROPS 20
#    endif

bool MACE_FALLING(effect_params_t* params) {
    const uint8_t drop_ticks = 26;
    const uint8_t decay_rate = 4;

    static uint8_t drop  = 0;
    static uint8_t decay = 0;

    if (params->init) {
        memset(g_rgb_frame_buffer, 0, sizeof(g_rgb_frame_buffer));
        drop  = 0;
        decay = 0;
    }

    decay++;
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            if (row == 0 && drop == 0 && rand() < RAND_MAX / MACE_RAIN_DROPS) {
                g_rgb_frame_buffer[row][col] = 255;
            } else if (g_rgb_frame_buffer[row][col] > 0 && g_rgb_frame_buffer[row][col] < 255) {
                if (decay >= decay_rate) {
                    g_rgb_frame_buffer[row][col]--;
                }
            }

            uint8_t led[LED_HITS_TO_REMEMBER];
            uint8_t led_count = rgb_matrix_map_row_column_to_led(row, col, led);
            if (led_count > 0) {
                hsv_t hsv = rgb_matrix_config.hsv;
                hsv.v     = g_rgb_frame_buffer[row][col];
                rgb_t rgb = rgb_matrix_hsv_to_rgb(hsv);
                for (uint8_t index = 0; index < led_count; index++) {
                    rgb_matrix_set_color(led[index], rgb.r, rgb.g, rgb.b);
                }
            }
        }
    }

    if (decay >= decay_rate) {
        decay = 0;
    }

    if (++drop > drop_ticks) {
        drop = 0;
        for (uint8_t row = MATRIX_ROWS - 1; row > 0; row--) {
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                if (row == MATRIX_ROWS - 1 && g_rgb_frame_buffer[row][col] == 255) {
                    g_rgb_frame_buffer[row][col] = 254;
                }
                if (g_rgb_frame_buffer[row - 1][col] >= 255) {
                    g_rgb_frame_buffer[row - 1][col] = 254;
                    g_rgb_frame_buffer[row][col]     = 255;
                }
            }
        }
    }

    return false;
}

/* ------------------------------------------------------------------ */
/* 6. Colourful                                                       */
/* ------------------------------------------------------------------ */

static hsv_t mace_colourful_math(hsv_t hsv, uint8_t i, uint8_t time) {
    uint8_t x = g_led_config.point[i].x;
    uint8_t y = g_led_config.point[i].y;

    hsv.h = (uint8_t)(x * 3 + y * 5 + time);
    hsv.s = 255;
    return hsv;
}

bool MACE_COLOURFUL(effect_params_t* params) {
    return effect_runner_i(params, &mace_colourful_math);
}

/* ------------------------------------------------------------------ */
/* 7. Breath                                                          */
/* ------------------------------------------------------------------ */

static hsv_t mace_breath_math(hsv_t hsv, uint8_t i, uint8_t time) {
    hsv.v = scale8(abs8(sin8(time / 2) - 128) * 2, hsv.v);
    return hsv;
}

bool MACE_BREATH(effect_params_t* params) {
    return effect_runner_i(params, &mace_breath_math);
}

/* ------------------------------------------------------------------ */
/* 8. Spectrum — full rainbow cycle                                   */
/* ------------------------------------------------------------------ */

static hsv_t mace_spectrum_math(hsv_t hsv, uint8_t i, uint8_t time) {
    hsv.h = time;
    hsv.s = 255;
    return hsv;
}

bool MACE_SPECTRUM(effect_params_t* params) {
    return effect_runner_i(params, &mace_spectrum_math);
}

/* ------------------------------------------------------------------ */
/* 9. Outward — rings from the centre                                 */
/* ------------------------------------------------------------------ */

static hsv_t mace_outward_math(hsv_t hsv, int16_t dx, int16_t dy, uint8_t dist, uint8_t time) {
    hsv.v = scale8(sin8(dist * 3 - time * 2), hsv.v);
    return hsv;
}

bool MACE_OUTWARD(effect_params_t* params) {
    return effect_runner_dx_dy_dist(params, &mace_outward_math);
}

/* ------------------------------------------------------------------ */
/* 10. Scrolling — top to bottom                                      */
/* ------------------------------------------------------------------ */

static hsv_t mace_scrolling_math(hsv_t hsv, uint8_t i, uint8_t time) {
    hsv.v = scale8(sin8(g_led_config.point[i].y * 4 - time * 2), hsv.v);
    return hsv;
}

bool MACE_SCROLLING(effect_params_t* params) {
    return effect_runner_i(params, &mace_scrolling_math);
}

/* ------------------------------------------------------------------ */
/* 11. Rolling — left to right                                        */
/* ------------------------------------------------------------------ */

static hsv_t mace_rolling_math(hsv_t hsv, uint8_t i, uint8_t time) {
    hsv.v = scale8(sin8(g_led_config.point[i].x * 2 - time * 3), hsv.v);
    return hsv;
}

bool MACE_ROLLING(effect_params_t* params) {
    return effect_runner_i(params, &mace_rolling_math);
}

/* ------------------------------------------------------------------ */
/* 12. Rotating                                                       */
/* ------------------------------------------------------------------ */

static hsv_t mace_rotating_math(hsv_t hsv, int16_t dx, int16_t dy, uint8_t time) {
    hsv.v = scale8(sin8(atan2_8(dy, dx) + time * 2), hsv.v);
    return hsv;
}

bool MACE_ROTATING(effect_params_t* params) {
    return effect_runner_dx_dy(params, &mace_rotating_math);
}

/* ------------------------------------------------------------------ */
/* 13. Explode                                                        */
/* ------------------------------------------------------------------ */

static hsv_t mace_explode_math(hsv_t hsv, int16_t dx, int16_t dy, uint8_t dist, uint8_t time) {
    hsv.v = scale8(sin8(dist * 3 - time * 3), hsv.v);
    return hsv;
}

bool MACE_EXPLODE(effect_params_t* params) {
    return effect_runner_dx_dy_dist(params, &mace_explode_math);
}

/* ------------------------------------------------------------------ */
/* 14. Launch — sweep from the bottom to the top                      */
/* ------------------------------------------------------------------ */

static hsv_t mace_launch_math(hsv_t hsv, uint8_t i, uint8_t time) {
    hsv.v = scale8(sin8((uint8_t)(g_led_config.point[i].y * 3 - time * 3)), hsv.v);
    return hsv;
}

bool MACE_LAUNCH(effect_params_t* params) {
    return effect_runner_i(params, &mace_launch_math);
}

/* ------------------------------------------------------------------ */
/* 15. Ripples — key-reactive expanding rings                         */
/* ------------------------------------------------------------------ */

static hsv_t mace_ripples_math(hsv_t hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
    uint16_t effect = tick - dist;
    if (effect > 255) effect = 255;

    hsv.v = qadd8(hsv.v, 255 - effect);
    return hsv;
}

bool MACE_RIPPLES(effect_params_t* params) {
    return effect_runner_reactive_splash(0, params, &mace_ripples_math);
}

/* ------------------------------------------------------------------ */
/* 16. Flowing — diagonal flowing wave                                */
/* ------------------------------------------------------------------ */

static hsv_t mace_flowing_math(hsv_t hsv, uint8_t i, uint8_t time) {
    uint8_t x = g_led_config.point[i].x;
    uint8_t y = g_led_config.point[i].y;

    hsv.v = scale8(sin8((uint8_t)(x + y * 2 + time)), hsv.v);
    return hsv;
}

bool MACE_FLOWING(effect_params_t* params) {
    return effect_runner_i(params, &mace_flowing_math);
}

/* ------------------------------------------------------------------ */
/* 17. Pulsating                                                      */
/* ------------------------------------------------------------------ */

static hsv_t mace_pulsating_math(hsv_t hsv, uint8_t i, uint8_t time) {
    hsv.v = scale8(abs8(sin8(time * 2) - 128) * 2, hsv.v);
    return hsv;
}

bool MACE_PULSATING(effect_params_t* params) {
    return effect_runner_i(params, &mace_pulsating_math);
}

/* ------------------------------------------------------------------ */
/* 18. Tilt — diagonal wave                                           */
/* ------------------------------------------------------------------ */

static hsv_t mace_tilt_math(hsv_t hsv, uint8_t i, uint8_t time) {
    uint8_t x = g_led_config.point[i].x;
    uint8_t y = g_led_config.point[i].y;

    hsv.v = scale8(sin8((uint8_t)(x * 2 - y * 3 - time * 2)), hsv.v);
    return hsv;
}

bool MACE_TILT(effect_params_t* params) {
    return effect_runner_i(params, &mace_tilt_math);
}

/* ------------------------------------------------------------------ */
/* 19. Shuttle — bright band moves left-right                         */
/* ------------------------------------------------------------------ */

static hsv_t mace_shuttle_math(hsv_t hsv, uint8_t i, uint8_t time) {
    uint8_t  pos  = scale8(sin8(time), 221);
    int16_t  diff = (int16_t)g_led_config.point[i].x - (int16_t)pos;
    uint16_t dist = diff < 0 ? (uint16_t)(-diff) : (uint16_t)diff;

    hsv.v = scale8(dist > 127 ? 0 : 255 - dist * 2, hsv.v);
    return hsv;
}

bool MACE_SHUTTLE(effect_params_t* params) {
    return effect_runner_i(params, &mace_shuttle_math);
}

/* ------------------------------------------------------------------ */
/* 20. Off — not one of the nineteen original modes, but useful       */
/* ------------------------------------------------------------------ */

bool MACE_OFF(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, 0, 0, 0);
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
