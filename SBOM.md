# Software Bill of Materials (SBOM) – CYFRAM Library

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)

**Version:** v1.2.0  
**Generated:** February 2026

---

## 1. Overview

This SBOM lists all components of the CYFRAM Arduino library:

- I²C/SPI FRAM Library
- Dependencies: Arduino Wire library
- License: GPL-2.0

---

## 2. Components

| Component | Version | License | Source / Link |
|-----------|--------|---------|---------------|
| CYFRAM Library | v1.2.0 | GPL-2.0 | [./src](./src) |
| Wire.h (Arduino) | 1.0 | Arduino License | [Arduino Core](https://github.com/arduino/ArduinoCore-avr) |

---

## 3. SPDX Example (JSON)

```json
{
  "SPDXID": "SPDXRef-DOCUMENT",
  "name": "CYFRAM Arduino Library SBOM",
  "documentNamespace": "https://github.com/artkeller/CYFRAM",
  "creationInfo": {
    "created": "2026-02-04T00:00:00Z",
    "creators": ["Person: artkeller"]
  },
  "packages": [
    {
      "SPDXID": "SPDXRef-CYFRAM",
      "name": "CYFRAM",
      "versionInfo": "1.2.0",
      "licenseConcluded": "GPL-2.0",
      "downloadLocation": "https://github.com/artkeller/CYFRAM"
    },
    {
      "SPDXID": "SPDXRef-Wire",
      "name": "Wire",
      "versionInfo": "1.0",
      "licenseConcluded": "Arduino",
      "downloadLocation": "https://github.com/arduino/ArduinoCore-avr"
    }
  ]
}

```

For machine-readable format see: [sbom.spdx.json](./sbom.spdx.json)
