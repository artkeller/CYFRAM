# Threat Model – CYFRAM Arduino Library

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)

**Scope:** I²C / SPI FRAM library for Arduino — covers both the Legacy
track (`src/`) and the Pro track (`pro/src/`).

---

## 1. Assets

| Asset | Description | Sensitivity |
|-------|------------|-------------|
| FRAM Memory | Non-volatile memory on I²C/SPI bus | Low (demo) |
| I²C / SPI Bus | Communication channel | Medium |
| MCU Host | Executes library | Low |

---

## 2. Threats & Risks

| Threat | Risk Level | Mitigation | Evidence |
|--------|-----------|-----------|----------|
| Bus sniffing / MITM | Low | Use in trusted environment, no network exposure | [README.md](./README.md#security-considerations) |
| Buffer overflow | Medium | Defensive coding, check bounds | [CODESTYLE.md](./CODESTYLE.md); Pro: explicit `checkRange()` bounds check before every bus access — [pro/src/CY_SPI_FRAM.cpp](./pro/src/CY_SPI_FRAM.cpp), [pro/src/CY_I2C_FRAM.cpp](./pro/src/CY_I2C_FRAM.cpp) |
| Null pointer / invalid address | Medium | NULL_PTR checks, error codes | [src/CYI2CFRAM.h](./src/CYI2CFRAM.h); Pro: NULL-checks on all burst methods, typed `Status` return — [pro/src/CY_SPI_FRAM.h](./pro/src/CY_SPI_FRAM.h), [pro/src/CY_I2C_FRAM.h](./pro/src/CY_I2C_FRAM.h) |
| Shared bus contention / interrupt-unsafe access (Pro-only finding) | Medium | SPI transaction locking (`beginTransaction`/`endTransaction`); mapped I2C `endTransmission()` error codes instead of silent failure | [pro/src/CY_SPI_FRAM.cpp](./pro/src/CY_SPI_FRAM.cpp), [pro/src/CY_I2C_FRAM.cpp](./pro/src/CY_I2C_FRAM.cpp) |

Note: the Legacy driver (`src/`) does not implement SPI transaction
locking or mapped I2C error codes; this is documented as a known
limitation of the frozen Legacy track, addressed in Pro. See
`MIGRATION.md`.

---

## 3. Security Controls

- Defensive coding practices ([CODESTYLE.md](./CODESTYLE.md))
- Limited buffer length (I2C_BUF_LEN, Legacy) / configurable chunked
  I2C transfers (`chunkBytes`, Pro)
- Clear error codes and return values (Pro: typed `Status` enum for
  every public method; Legacy: partial)
- SBOM for dependency transparency ([SBOM.md](./SBOM.md))

---

## 4. CRA References

- Article 13(1) Secure Development → [CODESTYLE.md](./CODESTYLE.md)
- Article 13(2) Risk Reduction → [README.md](./README.md#security-considerations)
- Article 13(3) SBOM → [SBOM.md](./SBOM.md)
