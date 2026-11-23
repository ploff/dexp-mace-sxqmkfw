# SonixQMK для DEXP Mace

**Версия:** `v0.3` · [История изменений](CHANGELOG.md)

Готовая прошивка QMK для клавиатуры **DEXP Mace (RKGK890)**, она же
**ARDOR Wakizashi** (по `device.xml`). 87 клавиш, TKL, per-key RGB.

# Характеристики

- MCU: **HFD2201KBA aka SONiX SN32F248B**, LQFP64, 64 KB flash, 8 KB RAM
- Матрица: **6 строк × 17 колонок**, `COL2ROW`
- Подсветка: **6 × 17 RGB Matrix**, 87 светодиодов, драйвер `sn32f2xx`
  (требуется патч из `qmk/patches/`)
- USB: `1209:0001`, строка «DEXP Mace»
- Bootloader: `0c45:7040` (Sonix ISP), вход через `Fn+Esc` или pin 3 → GND
- Прошивка: `firmware/dexp_mace_default.bin`

## Что работает

- ✅ Все клавиши.
- ✅ Мультимедиа-клавиши в `Fn`-строке как в оригинале.
- ✅ **Подсветка: все 87 светодиодов, все шесть строк**, 19 оригинальных
  режимов, цвет, яркость и скорость управляются с клавиатуры
  (см. [docs/lighting.md](docs/lighting.md)).
- ✅ `Fn+W` — обмен `WASD` со стрелками, состояние сохраняется в EEPROM.
- ✅ `Fn+Esc` — Sonix-бутлоадер.
- ⚠️ Индикаторы `Caps Lock` / `Scroll Lock` / `Win Lock` пока горят вместе
  с подсветкой — это отдельные одноцветные светодиоды, их управление
  появится в следующем релизе.
- ⚠️ VIA и дополнительные слои не настроены.

## Карта подсветки

| Ряд | Ноги U4 | QMK |
|---|---|---|
| F | 25, 26, 27 | A8, A9, A10 |
| цифры | 28, 29, 30 | A11, A12, A13 |
| QWERTY | 31, 32, 34 | A14, A15, B0 |
| ASDF | 35, 36, 37 | B1, B2, B3 |
| ZXCV | 38, 39, 41 | B4, B7, B5 |
| нижний | 51, 48, 44 | D3, B14, B10 |

Обязателен `SOFTWARE_PWM` — без него правый блок (`B12/B13/B15`) не
горит. Подробности, методика прозвонки и описание исправления артефакта
«столбец при нажатии» — в [docs/lighting.md](docs/lighting.md).

## Fn-клавиши

`Fn`-слой повторяет оригинальную инструкцию DEXP Mace (RKGK890).

| Комбинация | Действие | QMK-код |
|---|---|---|
| `Fn+F1` | Этот компьютер | `KC_MYCM` |
| `Fn+F2` | Запуск браузера | `KC_WHOM` |
| `Fn+F3` | Почта | `KC_MAIL` |
| `Fn+F4` | Проигрыватель | `KC_MSEL` |
| `Fn+F5` | Предыдущий трек | `KC_MPRV` |
| `Fn+F6` | Следующий трек | `KC_MNXT` |
| `Fn+F7` | Воспроизведение/пауза | `KC_MPLY` |
| `Fn+F8` | Остановка | `KC_MSTP` |
| `Fn+F9` | Громкость + | `KC_VOLU` |
| `Fn+F10` | Громкость − | `KC_VOLD` |
| `Fn+F11` | Отключение звука | `KC_MUTE` |
| `Fn+F12` | Калькулятор | `KC_CALC` |
| `Fn+Scroll Lock` | Режим подсветки | `RM_NEXT` |
| `Fn+Home` | Цвет подсветки | `MACE_COLOR_NEXT` |
| `Fn+↑` / `Fn+↓` | Яркость + / − | `RM_VALU` / `RM_VALD` |
| `Fn+←` / `Fn+→` | Скорость − / + | `RM_SPDD` / `RM_SPDU` |
| `Fn+W` | Поменять `WASD` и стрелки | `MACE_WASD_SWAP` |
| `Fn+Esc` | Бутлоадер (в оригинале — сброс настроек) | `QK_BOOT` |

## Матрица

```text
rows: D11 D10 D9 D8 D7 D6
cols: C12 C11 C10 C9 C8 C7 C6 C5 C4 C3 C2 C1 C0 C15 B15 B13 B12
```

| строка | назначение | колонки |
|---|---|---|
| `D11` | F-ряд | `C12…C0` = Esc, F1…F12; `B15/B13/B12` = PrtSc/ScrLk/Pause |
| `D10` | цифры | `C12…C0` = `` ` ``…`=`; `C15` = Backspace; `B15/B13/B12` = Ins/Home/PgUp |
| `D9` | QWERTY | `C12…C0` = Tab…`]`; `C15` = `\`; `B15/B13/B12` = Del/End/PgDn |
| `D8` | ASDF | `C12…C1` = Caps…`'`; `C15` = Enter |
| `D7` | ZXCV | `C12,C10…C1` = Shift, Z…`/`; `C15` = RShift; `B13` = ↑ |
| `D6` | нижний | `C12/C11/C10` = Ctrl/Win/Alt; `C6` = Space; `C2/C1/C0` = RAlt/Fn/Menu; `C15` = RCtrl; `B15/B13/B12` = ←/↓/→ |

Подробнее: [docs/matrix.md](docs/matrix.md).

## Сборка

Нужен форк **SonixQMK** (ветка `sn32_develop` или совместимая): в обычном
QMK нет SN32F248B и драйвера `sn32f2xx`.

```bash
cp -r qmk/keyboards/dexp/mace  <qmk_firmware>/keyboards/dexp/mace
cd <qmk_firmware>
git apply /path/to/qmk/patches/0001-sn32f2xx-shared-matrix.patch
qmk compile -kb dexp/mace -km default
```

Патч обязателен: без него при нажатии клавиши её столбец будет вспыхивать
на полной яркости.

Результат: `build/dexp_mace_default.bin`.

## Заливка

1. Войти в bootloader:
   - из QMK: **`Fn+Esc`**;
   - аппаратно: **pin 3 → GND** (третий сверху слева, pin 1 — скошенный
     верхний левый), затем USB, подержать 1–2 с.
2. Проверить: `lsusb | grep 0c45` → `0c45:7040`.
3. Прошить:

```bash
sonixflasher -v 0c45/7040 -f firmware/dexp_mace_default.bin
```

Или готовым скриптом (нужен `sonixflasher` в `PATH`):

```bash
./tools/flash.sh
```

## Файлы

```text
firmware/                         готовая прошивка (.bin/.hex)
qmk/keyboards/dexp/mace/          исходники QMK-клавиатуры
  config.h                        пины и параметры RGB Matrix
  rgb_matrix_kb.inc               19 оригинальных эффектов
  keymaps/default/keymap.c        раскладка и Fn-комбинации
  keyboard.json                   матрица, раскладка, 87 LEDs
qmk/patches/                      патч драйвера sn32f2xx
tools/flash.sh                    прошивка одной командой
tools/gen_mace_final.py           генератор базовой матрицы (legacy)
docs/matrix.md                    карта матрицы
docs/lighting.md                  карта RGB, режимы и методика
docs/probe/                       сырые логи прозвонки
```
