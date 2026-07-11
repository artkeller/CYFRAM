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

// ----------------------------------------------------------------------
// Function:    CY_SPI_FRAM (constructor)
// Arguments:   csPin - chip-select GPIO; capacityBytes - device capacity;
//              clockHz - SPI clock; spiBus - SPI bus instance to use
// Return:      -
// Purpose:     Stores configuration and derives the address width (2 or
//              3 bytes) from the declared capacity. Does not touch
//              hardware; see begin().
// ----------------------------------------------------------------------
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

// ----------------------------------------------------------------------
// Function:    begin
// Arguments:   -
// Return:      Status::OK
// Purpose:     Configures the CS pin and starts the SPI bus. Must be
//              called once before any other public method.
// ----------------------------------------------------------------------
CY_SPI_FRAM::Status CY_SPI_FRAM::begin()
{
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH); // deselect chip before the bus is even active
    _spi.begin();
    _initialized = true;
    return Status::OK;
}

// ----------------------------------------------------------------------
// Function:    select / deselect
// Arguments:   -
// Return:      -
// Purpose:     Drives the chip-select line low/high. Internal helpers
//              only; always called from within an SPI transaction.
// ----------------------------------------------------------------------
void CY_SPI_FRAM::select()
{
    digitalWrite(_csPin, LOW);
}

void CY_SPI_FRAM::deselect()
{
    digitalWrite(_csPin, HIGH);
}

// ----------------------------------------------------------------------
// Function:    sendAddress
// Arguments:   address - target F-RAM byte address
// Return:      -
// Purpose:     Clocks out the address in the device's configured width
//              (2 or 3 bytes, MSB first), as required by every SPI
//              F-RAM opcode that takes an address.
// ----------------------------------------------------------------------
void CY_SPI_FRAM::sendAddress(uint32_t address)
{
    if (_addrBytes == 3) {
        _spi.transfer((uint8_t)((address >> 16) & 0xFF));
    }
    _spi.transfer((uint8_t)((address >> 8) & 0xFF));
    _spi.transfer((uint8_t)(address & 0xFF));
}

// ----------------------------------------------------------------------
// Function:    writeEnableLatch
// Arguments:   -
// Return:      -
// Purpose:     Issues the WREN opcode. Required by the chip before every
//              WRITE or WRSR command; internal helper only.
// ----------------------------------------------------------------------
void CY_SPI_FRAM::writeEnableLatch()
{
    select();
    _spi.transfer(OP_WREN);
    deselect();
}

// ----------------------------------------------------------------------
// Function:    checkRange
// Arguments:   address - start address; length - number of bytes
// Return:      Status::OK, ERR_ZERO_LENGTH or ERR_ADDRESS_OUT_OF_RANGE
// Purpose:     Overflow-safe bounds check against the declared device
//              capacity. Called by every public read/write method
//              before any bytes are clocked onto the bus.
// ----------------------------------------------------------------------
CY_SPI_FRAM::Status CY_SPI_FRAM::checkRange(uint32_t address, size_t length) const
{
    if (length == 0) {
        return Status::ERR_ZERO_LENGTH;
    }
    if (address >= _capacity) {
        return Status::ERR_ADDRESS_OUT_OF_RANGE;
    }
    if (length > (_capacity - address)) {
        return Status::ERR_ADDRESS_OUT_OF_RANGE;
    }
    return Status::OK;
}

// ----------------------------------------------------------------------
// Function:    writeByte
// Arguments:   address - target address; data - byte to write;
//              verify - if true, reads back and compares
// Return:      Status::OK or an error code (see CY_SPI_FRAM.h)
// Purpose:     Writes a single byte inside one locked SPI transaction
//              (WREN + WRITE), with optional read-back verification.
// ----------------------------------------------------------------------
CY_SPI_FRAM::Status CY_SPI_FRAM::writeByte(uint32_t address, uint8_t data, bool verify)
{
    if (!_initialized) {
        return Status::ERR_NOT_INITIALIZED;
    }
    Status rangeStatus = checkRange(address, 1);
    if (rangeStatus != Status::OK) {
        return rangeStatus;
    }

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
        if (s != Status::OK) {
            return s;
        }
        if (readBack != data) {
            return Status::ERR_VERIFY_FAILED;
        }
    }
    return Status::OK;
}

// ----------------------------------------------------------------------
// Function:    readByte
// Arguments:   address - source address; data - output reference
// Return:      Status::OK or an error code
// Purpose:     Reads a single byte inside one locked SPI transaction.
// ----------------------------------------------------------------------
CY_SPI_FRAM::Status CY_SPI_FRAM::readByte(uint32_t address, uint8_t &data)
{
    if (!_initialized) {
        return Status::ERR_NOT_INITIALIZED;
    }
    Status rangeStatus = checkRange(address, 1);
    if (rangeStatus != Status::OK) {
        return rangeStatus;
    }

    _spi.beginTransaction(_spiSettings);
    select();
    _spi.transfer(OP_READ);
    sendAddress(address);
    data = _spi.transfer(0xFF);
    deselect();
    _spi.endTransaction();

    return Status::OK;
}

// ----------------------------------------------------------------------
// Function:    writeBurst
// Arguments:   address - start address; buffer - source data;
//              length - byte count; verify - if true, reads back and
//              compares every byte
// Return:      Status::OK or an error code
// Purpose:     Writes a contiguous block inside one locked SPI
//              transaction. Bounds- and NULL-checked before any bus
//              activity.
// ----------------------------------------------------------------------
CY_SPI_FRAM::Status CY_SPI_FRAM::writeBurst(uint32_t address, const uint8_t *buffer,
                                             size_t length, bool verify)
{
    if (!_initialized) {
        return Status::ERR_NOT_INITIALIZED;
    }
    if (buffer == nullptr) {
        return Status::ERR_NULL_POINTER;
    }
    Status rangeStatus = checkRange(address, length);
    if (rangeStatus != Status::OK) {
        return rangeStatus;
    }

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
        // Re-read byte-wise and compare; slower than a block compare but
        // needs no additional heap/stack buffer.
        for (size_t i = 0; i < length; ++i) {
            uint8_t readBack = 0;
            Status s = readByte(address + i, readBack);
            if (s != Status::OK) {
                return s;
            }
            if (readBack != buffer[i]) {
                return Status::ERR_VERIFY_FAILED;
            }
        }
    }
    return Status::OK;
}

// ----------------------------------------------------------------------
// Function:    readBurst
// Arguments:   address - start address; buffer - destination;
//              length - byte count
// Return:      Status::OK or an error code
// Purpose:     Reads a contiguous block inside one locked SPI
//              transaction. Bounds- and NULL-checked before any bus
//              activity.
// ----------------------------------------------------------------------
CY_SPI_FRAM::Status CY_SPI_FRAM::readBurst(uint32_t address, uint8_t *buffer, size_t length)
{
    if (!_initialized) {
        return Status::ERR_NOT_INITIALIZED;
    }
    if (buffer == nullptr) {
        return Status::ERR_NULL_POINTER;
    }
    Status rangeStatus = checkRange(address, length);
    if (rangeStatus != Status::OK) {
        return rangeStatus;
    }

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

// ----------------------------------------------------------------------
// Function:    writeStatusRegister
// Arguments:   value - byte to write into the status register
// Return:      Status::OK or an error code
// Purpose:     Writes the F-RAM status register (e.g. block-protect
//              bits) inside one locked SPI transaction.
// ----------------------------------------------------------------------
CY_SPI_FRAM::Status CY_SPI_FRAM::writeStatusRegister(uint8_t value)
{
    if (!_initialized) {
        return Status::ERR_NOT_INITIALIZED;
    }

    _spi.beginTransaction(_spiSettings);
    writeEnableLatch();

    select();
    _spi.transfer(OP_WRSR);
    _spi.transfer(value);
    deselect();
    _spi.endTransaction();

    return Status::OK;
}

// ----------------------------------------------------------------------
// Function:    readStatusRegister
// Arguments:   value - output reference
// Return:      Status::OK or an error code
// Purpose:     Reads the F-RAM status register inside one locked SPI
//              transaction.
// ----------------------------------------------------------------------
CY_SPI_FRAM::Status CY_SPI_FRAM::readStatusRegister(uint8_t &value)
{
    if (!_initialized) {
        return Status::ERR_NOT_INITIALIZED;
    }

    _spi.beginTransaction(_spiSettings);
    select();
    _spi.transfer(OP_RDSR);
    value = _spi.transfer(0xFF);
    deselect();
    _spi.endTransaction();

    return Status::OK;
}

// ----------------------------------------------------------------------
// Function:    readDeviceID
// Arguments:   idBuffer - destination; idLength - bytes to read
// Return:      Status::OK or an error code
// Purpose:     Presence/self-test via the RDID opcode. Not all
//              Cypress/Infineon F-RAM parts implement RDID; callers
//              should treat an all-0x00 or all-0xFF response as
//              "unsupported / no chip" (see CY_SPI_FRAM.h).
// ----------------------------------------------------------------------
CY_SPI_FRAM::Status CY_SPI_FRAM::readDeviceID(uint8_t *idBuffer, size_t idLength)
{
    if (!_initialized) {
        return Status::ERR_NOT_INITIALIZED;
    }
    if (idBuffer == nullptr) {
        return Status::ERR_NULL_POINTER;
    }
    if (idLength == 0) {
        return Status::ERR_ZERO_LENGTH;
    }

    _spi.beginTransaction(_spiSettings);
    select();
    _spi.transfer(OP_RDID);
    for (size_t i = 0; i < idLength; ++i) {
        idBuffer[i] = _spi.transfer(0xFF);
    }
    deselect();
    _spi.endTransaction();

    return Status::OK;
}

// ----------------------------------------------------------------------
// Function:    writeDisable
// Arguments:   -
// Return:      Status::OK or an error code
// Purpose:     Explicitly clears the write-enable latch (defensive,
//              optional use).
// ----------------------------------------------------------------------
CY_SPI_FRAM::Status CY_SPI_FRAM::writeDisable()
{
    if (!_initialized) {
        return Status::ERR_NOT_INITIALIZED;
    }

    _spi.beginTransaction(_spiSettings);
    select();
    _spi.transfer(OP_WRDI);
    deselect();
    _spi.endTransaction();

    return Status::OK;
}
