# CRA Audit Traceability Matrix – CYFRAM Library

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)

This matrix maps CRA requirements to concrete repository artifacts,
separately for the Legacy track (`src/`, frozen) and the Pro track
(`pro/src/`, actively maintained).

| CRA Reference | Requirement | Track | Repository Evidence | Compliance Level |
|--------------|------------|-------|---------------------|-----------------|
| Article 3(22) | Market placement definition | Both | [CRA.md](./CRA.md) | Voluntary – Full |
| Article 13(1) | Secure development | Legacy | [CODESTYLE.md](./CODESTYLE.md) | Voluntary – Partial |
| Article 13(1) | Secure development | Pro | [CODESTYLE.md](./CODESTYLE.md), [pro/src/](./pro/src/) (bounds-checked, transaction-safe, function-header-documented) | Voluntary – Full |
| Article 13(2) | Risk reduction | Legacy | [README.md](./README.md#security-considerations) | Voluntary – Partial |
| Article 13(2) | Risk reduction | Pro | [README.md](./README.md#security-considerations), [THREAT_MODEL.md](./THREAT_MODEL.md) (bus-contention finding closed) | Voluntary – Full |
| Article 13(3) | SBOM | Both | [SBOM.md](./SBOM.md), [sbom.spdx.json](./sbom.spdx.json) | Voluntary – Full |
| Article 14(1) | Vulnerability handling | Both | [VULNERABILITY_HANDLING.md](./VULNERABILITY_HANDLING.md) | Voluntary – Full |
| Article 14(2) | Coordinated disclosure | Both | [SECURITY.md](./SECURITY.md#reporting-a-vulnerability) | Voluntary – Full |
| Article 14(4) | Communication channel | Both | [SECURITY.md](./SECURITY.md#contact) | Voluntary – Full |
| Article 15 | Documentation | Legacy | [README.md](./README.md) | Voluntary – Partial |
| Article 15 | Documentation | Pro | [README.md](./README.md), [pro/README.md](./pro/README.md), [MIGRATION.md](./MIGRATION.md) | Voluntary – Full |
| Recitals | OSS clarification | Both | [CRA.md](./CRA.md) | Voluntary – Full |
