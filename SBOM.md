# Software Bill of Materials (SBOM)

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)

This document provides a Software Bill of Materials for this project
in accordance with CRA Article 13 and common industry practices.

Format: SPDX 2.3 (simplified)
Scope: Source code repository

---

## Primary Component

- Name: CYFRAM
- Type: Software Library
- Language: C / C++
- Interfaces: I2C, SPI
- License: Open Source (see LICENSE)

---

## Included Components

| Component | Version | License | Source |
|----------|---------|---------|--------|
| None (standalone library) | – | – | – |

---

## Notes

- No third-party binaries are included.
- Hardware interfaces (I2C, SPI) are abstracted.
- SBOM is maintained manually as PoC.

For machine-readable format see: [sbom.spdx.json](./sbom.spdx.json)
