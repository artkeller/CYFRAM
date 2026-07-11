# Manufacturer Self-Declaration – CYFRAM Library

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)

**Project:** CYFRAM Arduino Library (Legacy track `src/` + Pro track `pro/`)
**Maintainer:** Individual Contributor (Non-commercial)
**Date:** February 2026 (updated July 2026 for the Pro track)

## 1. Scope

This self-declaration documents **voluntary CRA compliance** for the
CYFRAM repository, although it is **not commercially distributed**. It
covers both the frozen Legacy driver and the additive, actively
maintained Pro driver (CYFRAM_Pro).

---

## 2. Compliance Summary

| CRA Requirement | Status | Evidence |
|-----------------|--------|----------|
| Secure Development (Art 13) | Voluntary | [CODESTYLE.md](./CODESTYLE.md); Pro implementation in [pro/src](./pro/src) |
| Risk Reduction (Art 13) | Voluntary | [README.md](./README.md#security-considerations); [THREAT_MODEL.md](./THREAT_MODEL.md) |
| SBOM (Art 13) | Voluntary | [SBOM.md](./SBOM.md) |
| Vulnerability Handling (Art 14) | Voluntary | [VULNERABILITY_HANDLING.md](./VULNERABILITY_HANDLING.md) |
| Coordinated Disclosure (Art 14) | Voluntary | [SECURITY.md](./SECURITY.md) |
| Communication Channel (Art 14) | Voluntary | [SECURITY.md](./SECURITY.md#contact) |
| Documentation (Art 15) | Voluntary | [README.md](./README.md), [pro/README.md](./pro/README.md) |

---

## 3. Notes

- All CRA obligations are implemented **voluntarily**, above and beyond OSS exemption.
- Neither the Legacy nor the Pro track is placed on the market in the
  course of a commercial activity by a legal person; both remain
  outside the CRA's scope of application in the strict legal sense
  (see [CRA.md](./CRA.md)). This declaration documents compliance
  measures taken anyway, on a voluntary basis.
- Suitable for **audit PoC**.
- This declaration is **non-commercial**.
- Full traceability: see [Audit-Traceability-Matrix.md](./Audit-Traceability-Matrix.md), which distinguishes compliance depth per track (Legacy: Partial in several areas; Pro: Full).

---

## 4. References

- [EU Cyber Resilience Act](https://eur-lex.europa.eu/eli/reg/2024/2847/oj)
- [CRA Open Source Guidance](https://digital-strategy.ec.europa.eu/en/policies/cra-open-source)
