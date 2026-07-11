# Version History / Changelog

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)

This document describes the **versioning** of the project following Semantic Versioning (SemVer).

```
CHANGELOG.md
├── v1.3.0 – Current
├── v1.2.0
├── v1.1.0
├── v1.0.0 – Initial
```
---

## v1.3.0 – Current
**Date:** 2026-07-11
**Changes:**
- Added `pro/` track: object-oriented, bus-transaction-safe SPI/I2C driver (`CY_SPI_FRAM`, `CY_I2C_FRAM`), versioned independently as **CYFRAM_Pro v2.0.0** (own `pro/library.properties`)
- Legacy `src/` (CYSPIFRAM, CYI2CFRAM) left **unchanged and frozen** — existing Library Manager installs and forks remain unaffected
- New: `MIGRATION.md` (Legacy → Pro upgrade guide)
- Governance documents extended to cover both tracks: `SBOM.md`, `SECURITY.md`, `THREAT_MODEL.md`, `Audit-Traceability-Matrix.md`, `CRA.md`, `MANUFACTURER_SELF_DECLARATION.md`
- README.md rewritten to document the dual-track repository structure

---

## v1.2.0
**Date:** 2026-02-01  
**Changes:**
- Repository adjustments (folder structure, permissions)
- Badge bar in README updated / enhanced (CRA, Language, License, Version, Code Style, Security, Arduino)
- New files created for badge links:
  - `LANGUAGE.md`
  - `CODESTYLE.md`
  - `SECURITY.md`
- General repository cleanup and structure improvements

---

## v1.1.0
**Date:** 2022-12-10  
**Changes:**
- README updated

---

## v1.0.0 – Initial
**Date:** 2017-08-27  
**Changes:**
- Initial project creation
- Fully functional version of the code
- Basic files and folder structure set up

---

## Versioning Notes
- **Major** → incompatible changes, major restructuring  
- **Minor** → new features, documentation updates, improvements  
- **Patch** → small bug fixes, cosmetic changes

> This changelog serves as the reference for all version badges in the README.
> Note: `CYFRAM_Pro` (in `pro/`) is versioned independently in `pro/library.properties`, since it is packaged as a separate, additively-installed Arduino library. Its releases are still recorded here for repository-level traceability.
