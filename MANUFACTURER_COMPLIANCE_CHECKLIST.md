# Manufacturer Compliance Checklist (CRA – Voluntary)

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)

This checklist documents voluntary alignment with the manufacturer obligations
defined in the EU Cyber Resilience Act (Regulation (EU) 2024/2847).

Status: Proof of Concept (PoC) – Training & Audit Readiness
Scope: covers both the Legacy track (`src/`, frozen) and the Pro track
(`pro/`, actively maintained). Checkbox state below is unchanged from
the prior review and reflects the repository as a whole, not a
per-track assessment — see `Audit-Traceability-Matrix.md` for the
per-track compliance-depth breakdown (Legacy: Partial in several
areas; Pro: Full).

---

## 1. Manufacturer Identification

- [ ] Manufacturer name documented
- [ ] Contact information provided
- [ ] Role clarification (manufacturer vs. OSS author)

Evidence:
- CRA.md
- README.md
- MANUFACTURER_SELF_DECLARATION.md

---

## 2. Secure Development Process (Article 13)

- [ ] Secure development lifecycle documented
- [ ] Coding guidelines defined
- [ ] Dependency usage controlled
- [ ] Security considerations documented

Evidence:
- CODESTYLE.md
- README.md (see "Security Considerations" section)
- pro/src/ (bounds-checked, transaction-safe implementation; function-header-documented per CODESTYLE.md)

---

## 3. Risk Assessment & Threat Awareness

- [ ] Intended use described
- [ ] Reasonably foreseeable misuse considered
- [ ] Security assumptions documented

Evidence:
- README.md
- SECURITY.md
- THREAT_MODEL.md (includes Pro-specific finding: shared bus contention / interrupt-unsafe access, mitigated in pro/src/)

---

## 4. Vulnerability Handling Process (Article 14)

- [ ] Vulnerability intake process defined
- [ ] Triage and severity assessment documented
- [ ] Fix and disclosure workflow defined
- [ ] Contact channel provided

Evidence:
- VULNERABILITY_HANDLING.md (now distinguishes Legacy vs. Pro remediation path)
- SECURITY.md (Supported Versions table now lists both tracks)

---

## 5. Software Bill of Materials (SBOM)

- [ ] Components inventoried
- [ ] Versions recorded
- [ ] License information included
- [ ] SPDX-compatible format used

Evidence:
- SBOM.md (now includes SPI.h and both CYFRAM_Pro components)
- sbom.spdx.json (now includes CYFRAM_Pro packages and SPI dependency; note open licensing-string inconsistency between Legacy "GPL-2.0" and Pro "GPL-2.0-or-later OR LGPL-2.1-or-later" entries, pending a maintainer decision)

---

## 6. Security Updates & Maintenance

- [ ] Update policy documented
- [ ] Scope and limitations stated
- [ ] End-of-support conditions defined

Evidence:
- SECURITY.md
- SECURITY_UPDATE_POLICY.md (documents that Legacy is intentionally frozen; feature/robustness work continues in Pro only)
- CHANGELOG.md (v1.3.0 documents the Pro track's introduction)

---

## 7. Documentation & Traceability

- [ ] CRA applicability documented
- [ ] Voluntary compliance stated
- [ ] Audit traceability available

Evidence:
- CRA.md (all four language sections — English, Deutsch, 日本語, 简体中文 — current as of v1.3.0, incl. Pro track)
- Audit-Traceability-Matrix.md (now includes a Track column: Legacy vs. Pro)
- MIGRATION.md (Legacy → Pro migration guidance)

---

## Declaration

This checklist demonstrates voluntary CRA-aligned manufacturer practices.
No commercial distribution is performed.

Date:          2026-02-04 (evidence links updated 2026-07-11 for the Pro track)
Maintainer:    Thomas Walloschke
