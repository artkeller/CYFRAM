# Threat Model – CYFRAM Arduino Library

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)

**Scope:** I²C / SPI FRAM library for Arduino

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
| Buffer overflow | Medium | Defensive coding, check bounds | [CODESTYLE.md](./CODESTYLE.md) |
| Null pointer / invalid address | Medium | NULL_PTR checks, error codes | [src/CYI2CFRAM.h](./src/CYI2CFRAM.h) |

---

## 3. Security Controls

- Defensive coding practices ([CODESTYLE.md](./CODESTYLE.md))
- Limited buffer length (I2C_BUF_LEN)
- Clear error codes and return values
- SBOM for dependency transparency ([SBOM.md](./SBOM.md))

---

## 4. CRA References

- Article 13(1) Secure Development → [CODESTYLE.md](./CODESTYLE.md)
- Article 13(2) Risk Reduction → [README.md](./README.md#security-considerations)
- Article 13(3) SBOM → [SBOM.md](./SBOM.md)
