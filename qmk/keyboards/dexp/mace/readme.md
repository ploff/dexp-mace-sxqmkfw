# DEXP Mace (RKGK890)

Custom QMK firmware for the **DEXP Mace** (RKGK890), also sold as
**ARDOR Wakizashi**. 87-key TKL with per-key RGB.

* MCU: SONiX **SN32F248B** (HFD2201KBA), LQFP64, 64 KB flash / 8 KB RAM
* Matrix: 6 rows × 17 columns, `COL2ROW`
* Lighting: 6 × 17 shared-matrix RGB, `sn32f2xx` driver, 87 LEDs
* USB: `1209:0001`, device name “DEXP Mace”
* Bootloader: Sonix ISP `0c45:7040` — `Fn+Esc` (`QK_BOOT`) or BOOT pin to GND

## Matrix

```text
rows: D11 D10 D9 D8 D7 D6
cols: C12 C11 C10 C9 C8 C7 C6 C5 C4 C3 C2 C1 C0 C15 B15 B13 B12
```

The right-hand columns are `B15` (PrtSc/Ins/Del/Left), `B13`
(ScrLk/Home/End/Up/Down) and `B12` (Pause/PgUp/PgDn/Right).
`B14` is not connected to the matrix.

## Lighting

The firmware reproduces the original 19 lighting modes from the HFD/RKGK890
firmware. All 87 LEDs and all six rows are mapped.

```text
Fn+Scroll Lock  next mode
Fn+Home         next colour
Fn+Up / Down    brightness
Fn+Left / Right speed
Fn+W            swap WASD with arrows
```

RGB row channels (`SN32F2XX_RGB_MATRIX_ROW_PINS`, order R/B/G):

| Row | U4 pins | QMK |
|---|---|---|
| F | 25, 26, 27 | A8, A9, A10 |
| digits | 28, 29, 30 | A11, A12, A13 |
| QWERTY | 31, 32, 34 | A14, A15, B0 |
| ASDF | 35, 36, 37 | B1, B2, B3 |
| ZXCV | 38, 39, 41 | B4, B7, B5 |
| bottom | 51, 48, 44 | D3, B14, B10 |

`SOFTWARE_PWM` is required (no hardware PWM on `B12/B13/B15`).
`MATRIX_UNSELECT_DRIVE_HIGH` prevents the pressed key from holding the
shared LED column low.

Known issue: the Caps/Scroll/Win indicator LEDs still light together with
the backlight; their control pins are not mapped in QMK yet.

## Layout

Standard ANSI TKL. `Fn+Esc` enters the bootloader. `Fn+F1…F12` follow the
original key legends: This PC, browser, mail, media player, previous/next,
play/pause, stop, volume +/−, mute, calculator. See
`keymaps/default/keymap.c`.

## Build

```bash
qmk compile -kb dexp/mace -km default
```

Requires the SonixQMK fork (`sn32_develop` or compatible) and the driver
patch `qmk/patches/0001-sn32f2xx-shared-matrix.patch` (without it the
pressed key's column flashes at full brightness).
