# Code Style Policy – Arduino Library

[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)

This document defines the **coding style** and conventions for this Arduino library repository.

---

## General Principles

- Code clarity is preferred over cleverness.
- Explicit behavior is preferred over implicit magic.
- Maintainable and readable code is mandatory, especially for embedded / low-level libraries.
- Comments should clearly explain **why** code exists, not what is obvious.

---

## File & Project Structure

- Library headers: `.h`  
- Implementation files: `.cpp`  
- File names should match class / module names where applicable.
- Each header must have include guards (`#ifndef ... #define ... #endif`).
- Public API should be clearly separated from internal functions.

---

## Formatting

- Indentation: 4 spaces per level (consistent throughout library)
- Braces: always use `{}` for code blocks, even for single-line statements
- Line length: 80–120 characters for readability
- End-of-file markers: optional, can use `//[] END OF FILE`

---

## Naming Conventions

- Constants / Macros: `UPPER_CASE`, e.g., `FRAM_I2C_MSTR_NO_ERROR`
- Functions: `UpperCamelCase` or `snake_case` (keep consistent in the project)
- Variables: `lowerCamelCase`
- Hardware pins / registers: follow datasheet terminology (`I2C_BUF_LEN`)

---

## Comments

- Use **English** for all comments.
- Block headers for functions should include:
  - Function name
  - Arguments
  - Return type
  - Purpose / usage
- Inline comments should be concise, explaining non-obvious logic.
- License headers must be included at the top of each file.

---

## Embedded / Low-Level Guidelines

- Hardware-related code (I2C, SPI, GPIO) must be explicitly commented.
- Timing-critical or interrupt-critical sections must be highlighted.
- Avoid dynamic memory allocation on constrained devices.
- Use macros or inline functions for repetitive low-level operations.

---

## Version Control / Commits

- Commit messages should be clear and descriptive.
- Follow semantic versioning for releases.
- Each release should correspond to updates in `CHANGELOG.md`.

---

## Review & Compliance

- All new code must follow this style.
- Exceptions must be justified in code comments.
- Code reviews include style and readability checks.

---

> This policy ensures **consistent, readable, and maintainable code** for the library across all contributors.
