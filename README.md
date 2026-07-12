# CYFRAM

Cypress/Infineon F-RAM Master Library for Arduino — SPI and I2C.

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)
[![CRA](https://img.shields.io/badge/CRA-voluntary%20compliance-blue)](./CRA.md)
[![Security](https://img.shields.io/badge/security-policy-orange)](./SECURITY.md)

This repository contains **two parallel, independently usable
libraries**:

| | Path | Status | For whom |
|---|---|---|---|
| **Legacy** | `src/`, `examples/` (repository root) | frozen, unchanged | existing Arduino Library Manager installs, existing forks, existing code using `#include <CYSPIFRAM.h>` |
| **Pro** | `pro/` | actively developed | new projects; anything that needs bus safety, error handling, multiple chips on one bus, or ESP32 support |

The Legacy library is **not removed or rewritten** — it stays exactly
as installed via the Library Manager, so no existing install or fork
breaks. The Pro library is an **additional** offering in its own
subfolder with its own `library.properties`.

## What's new in Pro?

Summary (full detail: [`CHANGELOG.md`](./CHANGELOG.md)):

- Object-oriented: multiple F-RAM chips on one bus instead of global state
- `SPI.beginTransaction()`/`endTransaction()` resp. mapped `Wire` error
  codes — bus- and interrupt-safe
- Automatic address width (SPI: 2/3 bytes, I2C: 1/2 bytes) based on
  declared chip capacity
- Bounds checking, NULL checks, optional read-back verify
- I2C: automatic chunking of burst transfers against the Wire buffer,
  clear error codes (address NACK vs. data NACK vs. bus error),
  `pingDevice()` self-test
- SPI: `readDeviceID()` self-test, configurable bus clock
- Examples for **AVR** (Uno/Nano/Mega, fixed SPI bus) and **ESP32**
  (freely mappable SPI/I2C pins, larger I2C chunk)

Pro costs more Flash than Legacy (roughly +750–1700 B depending on bus
and target) in exchange for bounds checking, typed error codes, and
bus-transaction safety — see [`SIZING.md`](./SIZING.md) for measured
figures (AVR/Uno; ESP32 not measured, see note there).

## Installation

**Legacy** (`CYFRAM`): as usual via the Arduino Library Manager, or as
a ZIP from the repository root.

**Pro** (`CYFRAM_Pro`): currently manual installation only, since the
Library Manager indexes only one root-level `library.properties` per
repository. Instructions: [`pro/README.md`](./pro/README.md).

## Security Considerations

Both tracks are maintained as a voluntary demonstration of security
practice, not under a commercial support contract. Key points:

- Input validation (bounds/NULL checks) is enforced in Pro on every
  public read/write method; Legacy has partial coverage only (see
  [`THREAT_MODEL.md`](./THREAT_MODEL.md) for the detailed gap analysis).
- Neither track has been fuzz-tested; both have been verified against
  real hardware via logic-analyzer capture, see
  `examples/FRAM_SPI_Example_2/logic/` and
  `examples/FRAM_I2C_Example_2/logic/` for the Legacy captures.
- This is a demo-scale library, not qualified for safety-related
  or life-critical use. See [`CRA_DISCLAIMER` section in `CRA.md`](./CRA.md)
  for the current legal scope assessment.
- Vulnerability reporting: [`SECURITY.md`](./SECURITY.md).

## Repository Structure

```
CYFRAM/
├── README.md                  <- this file
├── LICENSE                    <- GPLv2 / LGPLv2.1 (dual), unchanged
├── CHANGELOG.md                <- version history, both tracks
├── MIGRATION.md                 <- Legacy -> Pro migration guide
├── SIZING.md                     <- measured Flash/RAM footprint, Legacy vs. Pro
├── CODESTYLE.md                  <- coding conventions (applies to both tracks)
├── LANGUAGE.md                     <- documentation/code language policy
├── SECURITY.md                      <- vulnerability reporting
├── SECURITY_UPDATE_POLICY.md         <- release/patch process
├── VULNERABILITY_HANDLING.md          <- handling process detail
├── THREAT_MODEL.md                     <- assets, threats, mitigations
├── SBOM.md / sbom.spdx.json             <- software bill of materials
├── CRA.md                                <- CRA voluntary compliance (EN/DE/JA/ZH)
├── MANUFACTURER_COMPLIANCE_CHECKLIST.md   <- checklist
├── MANUFACTURER_SELF_DECLARATION.md        <- self-declaration
├── Audit-Traceability-Matrix.md             <- CRA article -> evidence mapping
├── library.properties                        <- Legacy, unchanged
├── src/                                        <- Legacy source (frozen)
│   ├── CYSPIFRAM.cpp / .h
│   └── CYI2CFRAM.cpp / .h
├── examples/                                    <- Legacy examples (frozen)
│   ├── FRAM_SPI_Example_2/  (incl. logic-analyzer captures)
│   └── FRAM_I2C_Example_2/  (incl. logic-analyzer captures)
└── pro/                                           <- Pro track (active development)
    ├── README.md
    ├── library.properties
    ├── keywords.txt
    ├── src/
    │   ├── CY_SPI_FRAM.h / .cpp
    │   └── CY_I2C_FRAM.h / .cpp
    └── examples/
        ├── FRAM_SPI_Pro_AVR_Uno/
        ├── FRAM_SPI_Pro_ESP32/
        ├── FRAM_I2C_Pro_AVR_Uno/
        └── FRAM_I2C_Pro_ESP32/
```

## Why this split?

A fork with production code on the old driver already exists. A
breaking change in the root path (renaming `src/CYSPIFRAM.h` or
changing its behavior) would silently break that fork and every other
existing install. The Pro library therefore lives as a standalone,
additive library in `pro/` — anyone who doesn't need it won't notice
it; anyone who wants it installs it deliberately.

## License

GPL-2.0-or-later OR LGPL-2.1-or-later (dual, your choice), as in the
original Cypress reference code. Applies to Legacy and Pro alike.

## Cyber Resilience Act (EU) 2024/2847

See [`CRA.md`](./CRA.md) and [`MANUFACTURER_SELF_DECLARATION.md`](./MANUFACTURER_SELF_DECLARATION.md)
for this project's voluntary compliance posture, and
[`Audit-Traceability-Matrix.md`](./Audit-Traceability-Matrix.md) for
the article-by-article evidence mapping (Legacy vs. Pro).
