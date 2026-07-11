# Security Update Policy – CYFRAM Library

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)

## 1. Purpose

Document update and patch process for CRA-related obligations in OSS.
Applies to both the Legacy track (`src/`, frozen — see note below) and
the Pro track (`pro/src/`, actively maintained).

---

## 2. Versioning

- Repository/Legacy follows semantic versioning: v1.0.0, v1.1.0, v1.2.0, v1.3.0…
- CYFRAM_Pro (`pro/`) is versioned independently, starting at v2.0.0, tracked in `pro/library.properties`
- Each release updates: CHANGELOG.md + SBOM.md

---

## 3. Update Triggers

- Security vulnerability found in library or dependency
- Bug fix affecting memory integrity
- Dependency updates (Arduino / Wire / SPI library)

**Legacy note:** the Legacy track is intentionally frozen (see
`README.md`, `MIGRATION.md`) to avoid breaking existing Library Manager
installs and forks. Legacy updates are limited to security-critical
fixes; feature and robustness improvements land in Pro only.

---

## 4. Deployment

- Releases published on GitHub
- Each release includes CHANGELOG.md, SBOM.md, and README.md update

---

## 5. Notification

- Users informed via GitHub releases
- Reference to VULNERABILITY_HANDLING.md if relevant

---

## 6. CRA References

- Article 14(1,2,4) → [VULNERABILITY_HANDLING.md](./VULNERABILITY_HANDLING.md)
- Article 13(3) → [SBOM.md](./SBOM.md)
