# CYFRAM_Pro

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](../LANGUAGE.md)

Object-oriented, bus-safe SPI/I2C driver for Cypress/Infineon F-RAM.
Second, additive track alongside the original `CYFRAM` (see `../src/`
in the repository root) — the Legacy library remains unchanged for
existing users and forks.

All design decisions and the full list of changes vs. the Legacy
implementation are in the repository [`CHANGELOG.md`](../CHANGELOG.md).

## Installation

The Arduino Library Manager indexes only one `library.properties` per
repository, **at the repository root**. Since this repository keeps
offering the Legacy library at the root, `CYFRAM_Pro` will **not**
appear automatically in the Library Manager. Manual installation:

1. Download this `pro/` folder, e.g. via `git sparse-checkout` or
   `svn export https://github.com/artkeller/CYFRAM/trunk/pro`.
2. Copy its contents to `~/Arduino/libraries/CYFRAM_Pro/`
   (`Documents/Arduino/libraries/CYFRAM_Pro` on Windows).
3. Restart the Arduino IDE.

Alternatively: clone the whole repository and use
*Sketch → Include Library → Add .ZIP Library* in the IDE, pointing at
a zipped `pro/` folder.

## Quickstart — SPI

```cpp
#include <SPI.h>
#include "CY_SPI_FRAM.h"

CY_SPI_FRAM fram(/*csPin=*/10, /*capacityBytes=*/32768UL);

void setup() {
    fram.begin();
    fram.writeByte(0x0000, 0xA5, /*verify=*/true);
}
```

## Quickstart — I2C

```cpp
#include <Wire.h>
#include "CY_I2C_FRAM.h"

CY_I2C_FRAM fram(/*deviceAddress=*/0x50, /*capacityBytes=*/32768UL);

void setup() {
    fram.begin();
    fram.writeByte(0x0000, 0xA5, /*verify=*/true);
}
```

Full board-specific examples (AVR and ESP32, SPI and I2C each) are in
`examples/`.

## Why a separate folder instead of replacing `src/`?

So that existing installs (Library Manager, forks, `#include
<CYSPIFRAM.h>`) do not break on updating to this state. See
[`../MIGRATION.md`](../MIGRATION.md) for moving from Legacy to Pro.

## Coding conventions

Follows the repository-wide [`CODESTYLE.md`](../CODESTYLE.md): English
comments, always braces around code blocks, function header blocks
documenting name/arguments/return/purpose, 4-space indentation.
