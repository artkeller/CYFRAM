# Software Bill of Materials (SBOM) – CYFRAM Library

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)

**Version:** v1.3.0
**Generated:** February 2026
**Updated:** July 2026 (added CYFRAM_Pro components)

---

## 1. Overview

This SBOM lists all components of the CYFRAM repository, covering both
the frozen Legacy track (`src/`) and the additive Pro track (`pro/src/`):

- I²C/SPI FRAM Library (Legacy: CYSPIFRAM, CYI2CFRAM)
- I²C/SPI FRAM Library, object-oriented (Pro: CY_SPI_FRAM, CY_I2C_FRAM)
- Dependencies: Arduino Wire library, Arduino SPI library
- License: GPL-2.0-or-later OR LGPL-2.1-or-later (dual)

---

## 2. Components

| Component | Version | License | Source / Link |
|-----------|--------|---------|---------------|
| CYFRAM Library (Legacy) | v1.2.0 | GPL-2.0-or-later OR LGPL-2.1-or-later | [./src](./src) |
| CYFRAM_Pro (CY_SPI_FRAM) | v2.0.0 | GPL-2.0-or-later OR LGPL-2.1-or-later | [./pro/src](./pro/src) |
| CYFRAM_Pro (CY_I2C_FRAM) | v2.0.0 | GPL-2.0-or-later OR LGPL-2.1-or-later | [./pro/src](./pro/src) |
| Wire.h (Arduino) | 1.0 | Arduino License | [Arduino Core](https://github.com/arduino/ArduinoCore-avr) |
| SPI.h (Arduino) | 1.0 | Arduino License | [Arduino Core](https://github.com/arduino/ArduinoCore-avr) |

---

## 3. SPDX Example (JSON)

```json
{
  "SPDXID": "SPDXRef-DOCUMENT",
  "name": "CYFRAM Arduino Library SBOM",
  "documentNamespace": "https://github.com/artkeller/CYFRAM",
  "creationInfo": {
    "created": "2026-07-11T00:00:00Z",
    "creators": ["Person: artkeller"]
  },
  "packages": [
    {
      "SPDXID": "SPDXRef-CYFRAM",
      "name": "CYFRAM",
      "versionInfo": "1.2.0",
      "licenseConcluded": "GPL-2.0-or-later OR LGPL-2.1-or-later",
      "downloadLocation": "https://github.com/artkeller/CYFRAM/tree/master/src"
    },
    {
      "SPDXID": "SPDXRef-CYFRAM-Pro",
      "name": "CYFRAM_Pro",
      "versionInfo": "2.0.0",
      "licenseConcluded": "GPL-2.0-or-later OR LGPL-2.1-or-later",
      "downloadLocation": "https://github.com/artkeller/CYFRAM/tree/master/pro"
    },
    {
      "SPDXID": "SPDXRef-Wire",
      "name": "Wire",
      "versionInfo": "1.0",
      "licenseConcluded": "Arduino",
      "downloadLocation": "https://github.com/arduino/ArduinoCore-avr"
    },
    {
      "SPDXID": "SPDXRef-SPI",
      "name": "SPI",
      "versionInfo": "1.0",
      "licenseConcluded": "Arduino",
      "downloadLocation": "https://github.com/arduino/ArduinoCore-avr"
    }
  ]
}
```

For machine-readable format see: [sbom.spdx.json](./sbom.spdx.json)

> **Pending:** `sbom.spdx.json` needs the same two `CYFRAM_Pro` package
> entries added manually (not regenerated here, to avoid overwriting the
> existing machine-readable file blind).
