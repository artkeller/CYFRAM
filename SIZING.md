# Memory Footprint — Legacy vs. Pro (SPI / I2C, AVR / ESP32)

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)

This document reports **measured** static (Flash/`.text`) and dynamic
(RAM/`.data`+`.bss`) memory footprint for the Legacy (`src/`) and Pro
(`pro/src/`) drivers, on real compiled firmware — not estimates.

## Method

- Toolchain: `avr-gcc` 7.3.0 (Atmel/AVR8 fork), `avr-libc` 2.0.0
- Core: [ArduinoCore-avr](https://github.com/arduino/ArduinoCore-avr) (upstream, unmodified), including its `SPI` and `Wire` libraries
- Target: `atmega328p` (Arduino Uno), `F_CPU=16000000L`
- Flags: `-Os -flto -ffunction-sections -fdata-sections`, link with `-Wl,--gc-sections` (i.e. the same optimization/dead-code-elimination behavior the Arduino IDE uses)
- Each variant links the **entire** Arduino core (as a real sketch would), plus the SPI and/or Wire library, plus the library under test, plus a test sketch that calls every public API function/method at least once (so nothing gets stripped as "unused")
- `avr-size -C` read from the final linked ELF; driver-only figures additionally taken from the isolated `.o` object file (pre-link, upper bound, before `--gc-sections` can remove anything)

Reproducibility note: every sketch explicitly includes `<Arduino.h>` before
any other header, exactly as the Arduino IDE auto-injects for `.ino`
files. Omitting it silently breaks C-linkage of `setup()`/`loop()` when
only `<Wire.h>` is included (`<Wire.h>` does not itself pull in
`Arduino.h`, unlike `<SPI.h>`) — this is a build-harness detail, not a
defect in either driver.

## Results — AVR (ATmega328p / Uno)

**Baseline** (Arduino core only, empty `setup()`/`loop()`, no FRAM library at all):

| | Flash | RAM |
|---|---|---|
| Baseline | 2038 B (6.2%) | 197 B (9.6%) |

**Full firmware** (core + bus library + FRAM driver + test sketch exercising every API call):

| Variant | Flash | RAM | Δ Flash vs. baseline | Δ RAM vs. baseline |
|---|---|---|---|---|
| Legacy — SPI (`CYSPIFRAM`) | 3012 B (9.2%) | 198 B (9.7%) | +974 B | +1 B |
| **Pro** — SPI (`CY_SPI_FRAM`) | 3762 B (11.5%) | 213 B (10.4%) | +1724 B | +16 B |
| Legacy — I2C (`CYI2CFRAM`) | 5552 B (16.9%) | 417 B (20.4%) | +3514 B | +220 B |
| **Pro** — I2C (`CY_I2C_FRAM`) | 6526 B (19.9%) | 436 B (21.3%) | +4488 B | +239 B |
| Pro — SPI **and** I2C combined | 7468 B (22.8%) | 452 B (22.1%) | +5430 B | +255 B |
| Legacy — SPI **and** I2C combined | *(not built — Legacy has no combined example; both drivers are trivially linkable together, expected ≈ sum minus shared core overhead, not separately verified)* | | | |

**Driver-only object file size** (isolated `.o`, before linker dead-code elimination — upper bound on what the driver itself contributes):

| Component | `.text` (Flash) | `.data`+`.bss` (RAM) |
|---|---|---|
| Legacy `CYSPIFRAM.cpp.o` | 604 B | 0 B |
| **Pro** `CY_SPI_FRAM.cpp.o` | 1462 B | 0 B |
| Legacy `CYI2CFRAM.cpp.o` | 1016 B | 0 B |
| **Pro** `CY_I2C_FRAM.cpp.o` | 1479 B | 0 B |

## Interpretation

- **Pro costs more Flash** — roughly +750 B (SPI) / +460 B (I2C driver
  object; +970 B at the linked-firmware level once `SPI.beginTransaction`
  bookkeeping is included) — this is the direct cost of: bounds
  checking, NULL checks, a typed `Status` return on every method,
  automatic address-width selection, `SPI.beginTransaction`/`endTransaction`
  locking, mapped I2C error codes, and I2C burst chunking. None of that
  exists in Legacy.
- **Neither driver holds meaningful static RAM of its own** — both
  object files show `0 B` `.data`/`.bss`. The RAM growth visible at the
  linked-firmware level for I2C (Legacy +220 B, Pro +239 B) comes
  almost entirely from the **Arduino `Wire`/`twi.c` library's own fixed
  TX/RX buffers**, not from either FRAM driver — this is a shared cost,
  not something Pro adds.
- **Both drivers together (Pro) still fit comfortably** on the smallest
  common AVR target (Uno: 32 KB Flash / 2 KB RAM) — 22.8% Flash / 22.1%
  RAM at worst case (SPI+I2C combined), leaving ample headroom for
  application code.
- On an ATmega328p with only 2 KB RAM, the ~250 B RAM figure for
  SPI+I2C-combined Pro is the number to watch if the application is
  already RAM-constrained (e.g. large buffers, multiple Serial/String
  use) — worth profiling per-project rather than assuming headroom.

## ESP32 — not measured in this environment

Qualitative expectation, **not a measured claim**: ESP32 boards
typically ship 4 MB+ Flash and 320 KB+ RAM, roughly two to three orders
of magnitude more headroom than an ATmega328p. The few-hundred-byte
Flash/RAM delta between Legacy and Pro measured on AVR above is very
unlikely to matter on ESP32 in practice — but this is an expectation
based on the general size class of the two platforms, not a
measurement, and should not be cited as one.

If real ESP32 numbers are needed, the reliable way to get them is
`arduino-cli compile --fqbn esp32:esp32:esp32 --show-properties` (or
just watching the compiler's own Flash/RAM summary) inside the actual
Arduino IDE / `arduino-cli` with the ESP32 board package installed —
happy to walk through that if you want to run it locally and share the
output for inclusion here.
