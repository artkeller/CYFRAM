# Security Policy

This document describes the **security policy** for this project, including supported versions, vulnerability reporting, and response procedures.

---

## Supported Versions

This project provides **security updates** only for the following versions:

| Version | Supported          |
| ------- | ----------------- |
| 1.2.0   | :white_check_mark: |
| < 1.2.0 | :x:                |

> Only the latest minor/patch release of each major version is actively maintained for security issues.

---

## Reporting a Vulnerability

If you discover a security vulnerability, please report it **privately**:

1. **[Send a security report](mailto:artkeller@gmx.de)** or use a private GitHub issue marked as **security**.  
2. Provide a clear description of the vulnerability, including:
   - Affected version(s)
   - Steps to reproduce
   - Potential impact
3. Do **not** disclose the vulnerability publicly until a fix is released.

We will acknowledge your report within **48 hours**.

---

## Response and Timeline

- Confirm receipt of report: **within 48 hours**  
- Evaluate severity and impact: **within 5 business days**  
- Provide a patch/fix or mitigation: **as soon as reasonably possible**, typically **within 2–4 weeks**  
- Notify the reporter when the vulnerability is fixed or if it cannot be fixed

---

## Scope

This policy applies to:
- All source code in this repository
- Dependencies included in the library distribution

This policy does **not** cover:
- User-side application code using the library
- External systems or devices interacting with the library

---

## Security Best Practices

- Always use the latest supported version (`1.2.0`)  
- Validate input and check I²C / SPI transactions in your code  
- Avoid using deprecated or unsafe functions

---

> Following this policy ensures that security issues are handled responsibly, maintaining trust for users and contributors.
