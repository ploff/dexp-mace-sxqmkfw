# Прошивка QMK для DEXP Mace 

**Версия:** `0.1.1-pre` · [История изменений](CHANGELOG.md)

Готовая прошивка QMK для клавиатуры **DEXP Mace (RKGK890)**, она же **ARDOR Wakizashi** (по `device.xml`).

# Характеристики

- MCU: **HFD2201KBA aka SONiX SN32F248B**, LQFP64, 64 KB flash, 8 KB RAM
- Матрица: **6 строк × 17 колонок**, `COL2ROW`
- USB: `1209:0001`, строка «DEXP Mace»
- Bootloader: `0c45:7040` (Sonix ISP), вход через `Fn+Esc` или pin 3 → GND
- Прошивка: `firmware/dexp_mace_default.bin`, flash checksum `0xbcd5`

## Что работает

- ✅ Все клавиши работают.
- ✅ Мультимедиа-клавиши в `Fn`-строке работают как в оригинале
  (см. таблицу ниже).
- ✅ `Fn+Esc` отправляет клавиатуру в бутлоадер (для перепрошивки без мороки).
- ⚠️ Подсветка прошивкой не управляется.
- ⚠️ VIA и дополнительные слои не настроены.

## Fn-клавиши

Раскладка `Fn`-слоя повторяет оригинальную инструкцию DEXP Mace
(RKGK890). В отличие от оригинала, `Fn+Esc` здесь не сбрасывает
настройки, а входит в Sonix-бутлоадер — подсветка всё равно не управляется
прошивкой.

| Комбинация | Действие | QMK-код |
|---|---|---|
| `Fn+F1` | Этот компьютер | `KC_MYCM` |
| `Fn+F2` | Запуск браузера | `KC_WHOM` |
| `Fn+F3` | Почта | `KC_MAIL` |
| `Fn+F4` | Проигрыватель | `KC_MSEL` |
| `Fn+F5` | Предыдущий трек | `KC_MPRV` |
| `Fn+F6` | Следующий трек | `KC_MNXT` |
| `Fn+F7` | Воспроизведение/пауза | `KC_MPLY` |
| `Fn+F8` | Остановка воспроизведения | `KC_MSTP` |
| `Fn+F9` | Громкость + | `KC_VOLU` |
| `Fn+F10` | Громкость − | `KC_VOLD` |
| `Fn+F11` | Отключение звука | `KC_MUTE` |
| `Fn+F12` | Калькулятор | `KC_CALC` |
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

## Установка

### Сборка

```bash
cp -r qmk/keyboards/dexp/mace  <qmk_firmware>/keyboards/dexp/mace
cd <qmk_firmware>
qmk compile -kb dexp/mace -km default
```

Результат: `build/dexp_mace_default.bin`.

Альтернативно матрица/раскладка генерируются скриптом:

```bash
python3 tools/gen_mace_final.py <qmk_firmware>
```

### Заливка на клавиатуру

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

Флаг `55 55 AA AA` в конце образа уже на месте.

## Файлы

```text
firmware/            готовая прошивка (.bin/.hex)
qmk/keyboards/dexp/mace/   исходники QMK-клавиатуры
tools/gen_mace_final.py    генератор матрицы/раскладки
tools/flash.sh             прошивка одной командой
docs/matrix.md             карта матрицы и методика поиска
docs/probe/                сырые логи прозвонки
CHECKSUMS.sha256
VERSION
```

## Благодарности

QMK Firmware, ChibiOS и SN32-порт, `SonixFlasherC`, а также раскладка
Quartz K552X как отправная точка.
