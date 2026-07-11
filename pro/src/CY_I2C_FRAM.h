/*
 * CY_I2C_FRAM.h
 *
 * SPDX-License-Identifier: GPL-2.0-or-later OR LGPL-2.1-or-later
 *
 * "Pro" rewrite of the Cypress/Infineon I2C F-RAM master driver
 * (FM24-series style parts). Companion to CY_SPI_FRAM.h, same design
 * principles applied to I2C.
 *
 * -----------------------------------------------------------------------
 * Design notes / differences vs. a naive I2C port:
 *
 *  1. Object-oriented: one instance per 7-bit device address -> several
 *     F-RAM chips (different address-pin strapping) on one bus.
 *  2. Automatic 1-byte vs. 2-byte word-address selection based on the
 *     declared device capacity (parts <=256 Byte use a single address
 *     byte, larger parts use two).
 *  3. I2C write/read chunking: the Arduino Wire library has a small,
 *     platform-dependent internal buffer (classically 32 Bytes on AVR,
 *     larger/configurable on ESP32 etc.). Bursts are automatically
 *     split into chunks that fit, instead of silently truncating or
 *     corrupting data on a naive single Wire.write() call.
 *  4. Every operation returns a Status code, including a faithful
 *     mapping of Wire::endTransmission() error codes (address NACK,
 *     data NACK, bus error, timeout) instead of failing silently.
 *  5. Bounds checking against the declared device capacity, NULL/zero
 *     length checks, optional read-back verification after writes.
 *  6. pingDevice() as a zero-length-write presence/self-test, since
 *     I2C F-RAM parts do not expose a generic SPI-style RDID opcode.
 *
 *  Note: Cypress/Infineon I2C F-RAM parts (unlike EEPROM) do not need
 *  a write-cycle delay after writing and have no software write-enable
 *  latch equivalent to SPI's WREN -- a plain addressed write is
 *  sufficient. Some very large-capacity I2C F-RAM variants encode
 *  additional address bits into the 7-bit device address itself
 *  (comparable to 24-series EEPROM block-select); that scheme is
 *  device-specific and is intentionally NOT covered here -- check your
 *  exact part's datasheet if you exceed a single 2-byte address space.
 * -----------------------------------------------------------------------
 */

#pragma once

#include <Arduino.h>
#include <Wire.h>

class CY_I2C_FRAM {
public:
    enum class Status : uint8_t {
        OK = 0,
        ERR_NOT_INITIALIZED,
        ERR_NULL_POINTER,
        ERR_ZERO_LENGTH,
        ERR_ADDRESS_OUT_OF_RANGE,
        ERR_VERIFY_FAILED,
        ERR_I2C_DATA_TOO_LONG,   // Wire::endTransmission() == 1
        ERR_I2C_NACK_ADDRESS,    // Wire::endTransmission() == 2
        ERR_I2C_NACK_DATA,       // Wire::endTransmission() == 3
        ERR_I2C_BUS_ERROR,       // Wire::endTransmission() == 4
        ERR_I2C_TIMEOUT,         // Wire::endTransmission() == 5 (core-dependent)
        ERR_I2C_READ_SHORT       // fewer bytes returned than requested
    };

    /**
     * @param deviceAddress 7-bit I2C device address (e.g. 0x50), as
     *                      strapped by the chip's address pins.
     * @param capacityBytes Usable capacity of the F-RAM chip in bytes.
     *                      Used for address-width selection and bounds
     *                      checking.
     * @param clockHz       I2C bus clock in Hz (default 400 kHz Fast
     *                      Mode, which all common Cypress/Infineon I2C
     *                      F-RAM parts support).
     * @param chunkBytes    Max. bytes per Wire transaction chunk. Leave
     *                      at default unless you know your platform's
     *                      Wire buffer is smaller/larger than 32 Bytes.
     * @param wireBus       TwoWire bus instance to use (default: Wire).
     */
    explicit CY_I2C_FRAM(uint8_t deviceAddress,
                          uint32_t capacityBytes = 32768UL,
                          uint32_t clockHz = 400000UL,
                          size_t chunkBytes = 30,
                          TwoWire &wireBus = Wire);

    /// Initializes the I2C bus. Must be called once before use.
    Status begin();

    /// Zero-length-write presence/self-test ("is a device ACKing here?").
    Status pingDevice();

    Status writeByte(uint32_t address, uint8_t data, bool verify = false);
    Status readByte(uint32_t address, uint8_t &data);

    Status writeBurst(uint32_t address, const uint8_t *buffer, size_t length,
                       bool verify = false);
    Status readBurst(uint32_t address, uint8_t *buffer, size_t length);

    uint32_t capacity() const { return _capacity; }
    bool isInitialized() const { return _initialized; }

private:
    TwoWire &_wire;
    const uint8_t _deviceAddress;
    const uint32_t _capacity;
    const uint32_t _clockHz;
    const size_t _chunkBytes;
    uint8_t _addrBytes; // 1 or 2, derived from _capacity
    bool _initialized;

    void writeAddress(uint32_t address);
    Status checkRange(uint32_t address, size_t length) const;
    Status mapEndTransmission(uint8_t code) const;

    // Writes one chunk (<= _chunkBytes payload) starting at `address`.
    Status writeChunk(uint32_t address, const uint8_t *buffer, size_t length);
    // Reads one chunk (<= _chunkBytes payload) starting at `address`.
    Status readChunk(uint32_t address, uint8_t *buffer, size_t length);
};
