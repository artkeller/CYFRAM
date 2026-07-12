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

## I2C: mapping functions to methods

| Legacy (global) | Pro (method on instance) | Difference |
|---|---|---|
| `FRAM_I2C_Init()` | `fram.begin()` | returns `Status` instead of `void` |
| `FRAM_I2C_Write(slave, addr, ptr, n)` | `fram.writeByte(addr, data)` / `fram.writeBurst(addr, ptr, n)` | bounds-checked; automatic chunking against the Wire buffer instead of manual 30-byte splitting; optional `verify=true` |
| `FRAM_I2C_Random_Read(slave, addr, ptr, n)` | `fram.readByte(addr, data)` / `fram.readBurst(addr, ptr, n)` | bounds-checked; automatic chunking |
| `FRAM_I2C_Current_Read(slave, ptr, n)` | **intentionally not carried over** | ⚠️ see rationale below |
| global 7-bit slave address per call | constructor parameter `deviceAddress` | multiple instances/chips possible |

> **⚠️ Deliberate design exclusion, not an oversight:** Legacy's
> `FRAM_I2C_Current_Read()` reads from wherever the F-RAM's internal
> address pointer currently sits, without sending an address — a
> hardware-protocol shortcut from an era when saving two address bytes
> per read mattered. It is demonstrated in
> `examples/FRAM_I2C_Example_2/FRAM_I2C_Example_2.ino`.
>
> The address pointer lives **inside the chip, not in your program**,
> and I2C has no bus arbitration to protect it: any other access to the
> same chip — another part of your code, an ISR, a second bus master, a
> retried transaction — silently moves the pointer without your next
> `Current_Read()` call noticing. The read still ACKs; it just returns
> plausible-looking data from the wrong address, with no error code to
> catch it. That is incompatible with Pro's core design goal (every
> operation bounds-checked and explicitly addressed) and with the
> shared-bus-safety finding documented in `THREAT_MODEL.md` — you
> cannot bounds-check a read whose address you don't know.
>
> `CY_I2C_FRAM` therefore does not offer an equivalent, by design. If a
> genuine need for sequential streaming reads comes up later, the right
> answer is a separate, explicitly-named method (e.g.
> `readSequentialUnsafe()`) that carries the risk in its name, not a
> silent drop-in replacement for `readByte()`/`readBurst()`. Code that
> relies on `Current_Read()` today can keep using Legacy for that path.

## I2C: automatic chunking

If your own code currently splits large bursts by hand into ≤30-byte
pieces to stay under the Wire buffer limit (see `CYI2CFRAM.h`'s
`I2C_BUF_LEN`), `CY_I2C_FRAM` does that automatically — drop the manual
splitting and call `writeBurst()`/`readBurst()` with the full length.

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
