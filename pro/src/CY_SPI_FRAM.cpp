/*
 * CY_SPI_FRAM.cpp
 *
 * SPDX-License-Identifier: GPL-2.0-or-later OR LGPL-2.1-or-later
 * See CY_SPI_FRAM.h for provenance and the list of changes vs. the
 * original CYSPIFRAM.cpp.
 */

#include "CY_SPI_FRAM.h"

namespace {
    // Cypress/Infineon F-RAM SPI opcodes
    constexpr uint8_t OP_WREN  = 0x06; // Write Enable
    constexpr uint8_t OP_WRDI  = 0x04; // Write Disable
    constexpr uint8_t OP_RDSR  = 0x05; // Read Status Register
    constexpr uint8_t OP_WRSR  = 0x01; // Write Status Register
    constexpr uint8_t OP_READ  = 0x03; // Read Memory
    constexpr uint8_t OP_WRITE = 0x02; // Write Memory
    constexpr uint8_t OP_RDID  = 0x9F; // Read Device ID (not all parts)

    constexpr uint32_t ADDR_WIDTH_2_LIMIT = 65536UL; // <=64KiB -> 2-byte addr
}

CY_SPI_FRAM::CY_SPI_FRAM(uint8_t csPin, uint32_t capacityBytes,
                          uint32_t clockHz, SPIClass &spiBus)
    : _spi(spiBus),
      _spiSettings(clockHz, MSBFIRST, SPI_MODE0),
      _csPin(csPin),
      _capacity(capacityBytes),
      _addrBytes(capacityBytes > ADDR_WIDTH_2_LIMIT ? 3 : 2),
      _initialized(false)
{
}

CY_SPI_FRAM::Status CY_SPI_FRAM::begin()
{
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH); // deselect chip before the bus is even active
    _spi.begin();
    _initialized = true;
    return Status::OK;
}

void CY_SPI_FRAM::select()   { digitalWrite(_csPin, LOW); }
void CY_SPI_FRAM::deselect() { digitalWrite(_csPin, HIGH); }

void CY_SPI_FRAM::sendAddress(uint32_t address)
{
    if (_addrBytes == 3) {
        _spi.transfer((uint8_t)((address >> 16) & 0xFF));
    }
    _spi.transfer((uint8_t)((address >> 8) & 0xFF));
    _spi.transfer((uint8_t)(address & 0xFF));
}

void CY_SPI_FRAM::writeEnableLatch()
{
    select();
    _spi.transfer(OP_WREN);
    deselect();
}

CY_SPI_FRAM::Status CY_SPI_FRAM::checkRange(uint32_t address, size_t length) const
{
    if (length == 0) return Status::ERR_ZERO_LENGTH;
    // Overflow-safe range check (address + length can overflow uint32_t
    // on pathological inputs, so check via subtraction against capacity).
    if (address >= _capacity) return Status::ERR_ADDRESS_OUT_OF_RANGE;
    if (length > (_capacity - address)) return Status::ERR_ADDRESS_OUT_OF_RANGE;
    return Status::OK;
}

CY_SPI_FRAM::Status CY_SPI_FRAM::writeByte(uint32_t address, uint8_t data, bool verify)
{
    if (!_initialized) return Status::ERR_NOT_INITIALIZED;
    Status rangeStatus = checkRange(address, 1);
    if (rangeStatus != Status::OK) return rangeStatus;

    _spi.beginTransaction(_spiSettings);
    writeEnableLatch();

    select();
    _spi.transfer(OP_WRITE);
    sendAddress(address);
    _spi.transfer(data);
    deselect();
    _spi.endTransaction();

    if (verify) {
        uint8_t readBack = 0;
        Status s = readByte(address, readBack);
        if (s != Status::OK) return s;
        if (readBack != data) return Status::ERR_VERIFY_FAILED;
    }
    return Status::OK;
}

CY_SPI_FRAM::Status CY_SPI_FRAM::readByte(uint32_t address, uint8_t &data)
{
    if (!_initialized) return Status::ERR_NOT_INITIALIZED;
    Status rangeStatus = checkRange(address, 1);
    if (rangeStatus != Status::OK) return rangeStatus;

    _spi.beginTransaction(_spiSettings);
    select();
    _spi.transfer(OP_READ);
    sendAddress(address);
    data = _spi.transfer(0xFF);
    deselect();
    _spi.endTransaction();

    return Status::OK;
}

CY_SPI_FRAM::Status CY_SPI_FRAM::writeBurst(uint32_t address, const uint8_t *buffer,
                                             size_t length, bool verify)
{
    if (!_initialized) return Status::ERR_NOT_INITIALIZED;
    if (buffer == nullptr) return Status::ERR_NULL_POINTER;
    Status rangeStatus = checkRange(address, length);
    if (rangeStatus != Status::OK) return rangeStatus;

    _spi.beginTransaction(_spiSettings);
    writeEnableLatch();

    select();
    _spi.transfer(OP_WRITE);
    sendAddress(address);
    for (size_t i = 0; i < length; ++i) {
        _spi.transfer(buffer[i]);
    }
    deselect();
    _spi.endTransaction();

    if (verify) {
        // Verified in reasonably sized chunks on the caller's own buffer
        // would need extra RAM; instead we re-read byte-wise and compare,
        // which is slower but requires no additional heap/stack buffer.
        for (size_t i = 0; i < length; ++i) {
            uint8_t readBack = 0;
            Status s = readByte(address + i, readBack);
            if (s != Status::OK) return s;
            if (readBack != buffer[i]) return Status::ERR_VERIFY_FAILED;
        }
    }
    return Status::OK;
}

CY_SPI_FRAM::Status CY_SPI_FRAM::readBurst(uint32_t address, uint8_t *buffer, size_t length)
{
    if (!_initialized) return Status::ERR_NOT_INITIALIZED;
    if (buffer == nullptr) return Status::ERR_NULL_POINTER;
    Status rangeStatus = checkRange(address, length);
    if (rangeStatus != Status::OK) return rangeStatus;

    _spi.beginTransaction(_spiSettings);
    select();
    _spi.transfer(OP_READ);
    sendAddress(address);
    for (size_t i = 0; i < length; ++i) {
        buffer[i] = _spi.transfer(0xFF);
    }
    deselect();
    _spi.endTransaction();

    return Status::OK;
}

CY_SPI_FRAM::Status CY_SPI_FRAM::writeStatusRegister(uint8_t value)
{
    if (!_initialized) return Status::ERR_NOT_INITIALIZED;

    _spi.beginTransaction(_spiSettings);
    writeEnableLatch();

    select();
    _spi.transfer(OP_WRSR);
    _spi.transfer(value);
    deselect();
    _spi.endTransaction();

    return Status::OK;
}

CY_SPI_FRAM::Status CY_SPI_FRAM::readStatusRegister(uint8_t &value)
{
    if (!_initialized) return Status::ERR_NOT_INITIALIZED;

    _spi.beginTransaction(_spiSettings);
    select();
    _spi.transfer(OP_RDSR);
    value = _spi.transfer(0xFF);
    deselect();
    _spi.endTransaction();

    return Status::OK;
}

CY_SPI_FRAM::Status CY_SPI_FRAM::readDeviceID(uint8_t *idBuffer, size_t idLength)
{
    if (!_initialized) return Status::ERR_NOT_INITIALIZED;
    if (idBuffer == nullptr) return Status::ERR_NULL_POINTER;
    if (idLength == 0) return Status::ERR_ZERO_LENGTH;

    _spi.beginTransaction(_spiSettings);
    select();
    _spi.transfer(OP_RDID);
    for (size_t i = 0; i < idLength; ++i) {
        idBuffer[i] = _spi.transfer(0xFF);
    }
    deselect();
    _spi.endTransaction();

    // Not all Cypress/Infineon F-RAM parts implement RDID. Callers should
    // treat an all-0x00 or all-0xFF response as "unsupported / no chip".
    return Status::OK;
}

CY_SPI_FRAM::Status CY_SPI_FRAM::writeDisable()
{
    if (!_initialized) return Status::ERR_NOT_INITIALIZED;

    _spi.beginTransaction(_spiSettings);
    select();
    _spi.transfer(OP_WRDI);
    deselect();
    _spi.endTransaction();

    return Status::OK;
}
