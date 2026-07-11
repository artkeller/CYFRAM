# Migration: CYFRAM (Legacy) → CYFRAM_Pro

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)

This guide is for existing users of the Legacy library
(`#include <CYSPIFRAM.h>` / `<CYI2CFRAM.h>`) who want to move to the
object-oriented Pro driver. Migration is **optional** — Legacy remains
functional and unchanged.

## SPI: mapping functions to methods

| Legacy (global) | Pro (method on instance) | Difference |
|---|---|---|
| `FRAM_SPI_Init()` | `fram.begin()` | returns `Status` instead of `void` |
| `FRAM_SPI_Write(addr, data)` | `fram.writeByte(addr, data)` | optional `verify=true`; bounds-checked; returns `Status` |
| `FRAM_SPI_Read(addr)` | `fram.readByte(addr, data)` | value passed by reference so a `Status` can be returned |
| `FRAM_SPI_BurstWrite(addr, ptr, n)` | `fram.writeBurst(addr, ptr, n)` | bounds-checked against declared capacity; optional `verify=true` |
| `FRAM_SPI_BurstRead(addr, ptr, n)` | `fram.readBurst(addr, ptr, n)` | bounds-checked |
| `FRAM_SPI_Status_Reg_Write(v)` | `fram.writeStatusRegister(v)` | returns `Status` |
| `FRAM_SPI_Status_Reg_Read()` | `fram.readStatusRegister(v)` | value passed by reference |
| *(not available)* | `fram.readDeviceID(buf, len)` | new: chip-presence self-test |
| global `CS` pin macro | constructor parameter `csPin` | multiple instances/chips possible |

**Before (Legacy):**
```cpp
#include <SPI.h>
#include <CYSPIFRAM.h>

void setup() {
  FRAM_SPI_Init();
  FRAM_SPI_Write(0x0000, 0xA5);
}
```

**After (Pro):**
```cpp
#include <SPI.h>
#include "CY_SPI_FRAM.h"

CY_SPI_FRAM fram(/*csPin=*/10, /*capacityBytes=*/32768UL);

void setup() {
  fram.begin();
  fram.writeByte(0x0000, 0xA5, /*verify=*/true);
}
```

## I2C: analogous, plus automatic chunking

If your own code calls the Legacy I2C API directly: `CY_I2C_FRAM`
additionally chunks burst transfers automatically against the Wire
buffer (see `CHANGELOG.md`, "I2C variant" section). Existing code that
currently splits large bursts by hand can drop that logic and call
`writeBurst()`/`readBurst()` directly with the full length.

## Why it can be worth switching

- Return values surface errors that Legacy has so far swallowed
  silently (e.g. address overflow, NULL buffer).
- `SPI.beginTransaction()`/`endTransaction()` resp. clean I2C error
  codes make the driver safe on a shared bus with other peripherals.
- Multiple F-RAM chips on one bus are possible without forking the
  library.

## Why you might not need to switch (yet)

- Legacy keeps working unchanged for simple single-chip setups without
  bus contention.
- The Pro API is not binary-compatible and requires updating call
  sites in your own code (see table above) — weigh that against the
  benefit for stable, already-tested production code.

## Security-relevant difference

`THREAT_MODEL.md` documents a Pro-only finding ("shared bus contention
/ interrupt-unsafe access") that Legacy does not mitigate. If your
project shares the SPI/I2C bus with other peripherals or runs
interrupt-driven code, this is the strongest technical reason to
migrate.
