/*
 * CY_SPI_FRAM.h
 *
 * SPDX-License-Identifier: GPL-2.0-or-later OR LGPL-2.1-or-later
 *
 * "Pro" rewrite of the Cypress/Infineon SPI F-RAM master driver.
 * Original reference code: Copyright (c) 2014 Cypress Semiconductor
 *                           Technology Pvt Ltd. <medu@cypress.com>
 * Arduino port:            Copyright (c) 2017 artkeller@gmx.de
 * This revision:           object-oriented, transaction-safe rewrite.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 *
 * -----------------------------------------------------------------------
 * Changes vs. the original CYSPIFRAM.cpp/.h:
 *
 *  1. SPI.beginTransaction()/endTransaction() around every logical
 *     operation -> bus-safe on shared SPI buses, interrupt-safe.
 *  2. Object-oriented: one instance per chip-select pin/device, no
 *     global state -> multiple F-RAM chips on one bus are supported.
 *  3. Automatic 2-byte vs. 3-byte addressing depending on the declared
 *     device capacity (correct behaviour for >64 KBit parts instead of
 *     silently truncating the address).
 *  4. Bounds checking: every read/write is checked against the declared
 *     device capacity; burst operations are checked against buffer
 *     length AND remaining address space.
 *  5. Every operation returns a Status code (OK / error) instead of
 *     failing silently; optional read-back verification after writes.
 *  6. NULL-pointer and zero-length checks on all buffer operations.
 *  7. Configurable SPI clock (SPISettings) instead of relying on
 *     whatever default clock SPI.begin() happens to leave configured.
 *  8. Device-ID read (RDID) and Write-Disable (WRDI) added for parts
 *     that support them, so callers can detect a missing/faulty chip.
 * -----------------------------------------------------------------------
 */

#pragma once

#include <Arduino.h>
#include <SPI.h>

class CY_SPI_FRAM {
public:
    /// Result codes returned by every public operation.
    enum class Status : uint8_t {
        OK = 0,
        ERR_NOT_INITIALIZED,
        ERR_NULL_POINTER,
        ERR_ZERO_LENGTH,
        ERR_ADDRESS_OUT_OF_RANGE,
        ERR_VERIFY_FAILED,
        ERR_UNSUPPORTED
    };

    /**
     * @param csPin         Chip-select pin for this F-RAM device.
     * @param capacityBytes Usable capacity of the F-RAM chip in bytes
     *                      (e.g. 32768UL for a 256 Kbit part). Used for
     *                      address-width selection and bounds checking.
     * @param clockHz       SPI clock in Hz (default 20 MHz, safe for
     *                      most Cypress/Infineon F-RAM parts; lower it
     *                      for long wires / breadboards).
     * @param spiBus        SPI bus instance to use (default: SPI).
     */
    explicit CY_SPI_FRAM(uint8_t csPin,
                          uint32_t capacityBytes = 32768UL,
                          uint32_t clockHz = 20000000UL,
                          SPIClass &spiBus = SPI);

    /// Initializes the CS pin and the SPI bus. Must be called once before use.
    Status begin();

    /// Writes a single byte. If verify=true, reads it back and compares.
    Status writeByte(uint32_t address, uint8_t data, bool verify = false);

    /// Reads a single byte into `data`.
    Status readByte(uint32_t address, uint8_t &data);

    /// Writes `length` bytes from `buffer` starting at `address`.
    Status writeBurst(uint32_t address, const uint8_t *buffer, size_t length,
                       bool verify = false);

    /// Reads `length` bytes into `buffer` starting at `address`.
    Status readBurst(uint32_t address, uint8_t *buffer, size_t length);

    /// Writes the F-RAM status register (e.g. block-protect bits).
    Status writeStatusRegister(uint8_t value);

    /// Reads the F-RAM status register.
    Status readStatusRegister(uint8_t &value);

    /**
     * Reads the JEDEC/RDID device identification (if supported by the
     * chip). Useful as a "is the chip actually present and responding"
     * self-test. Returns ERR_UNSUPPORTED only if the read could not be
     * clocked out at all (e.g. bus not initialized).
     */
    Status readDeviceID(uint8_t *idBuffer, size_t idLength);

    /// Explicitly clears the write-enable latch (defensive/optional).
    Status writeDisable();

    /// Declared usable capacity in bytes.
    uint32_t capacity() const { return _capacity; }

    /// True once begin() has succeeded.
    bool isInitialized() const { return _initialized; }

private:
    SPIClass  &_spi;
    SPISettings _spiSettings;
    const uint8_t _csPin;
    const uint32_t _capacity;
    uint8_t _addrBytes;   // 2 or 3, derived from _capacity
    bool _initialized;

    void select();
    void deselect();
    void sendAddress(uint32_t address);
    void writeEnableLatch();
    Status checkRange(uint32_t address, size_t length) const;
};
