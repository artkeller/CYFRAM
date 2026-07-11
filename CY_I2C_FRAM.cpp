/*
 * CY_I2C_FRAM.cpp
 *
 * SPDX-License-Identifier: GPL-2.0-or-later OR LGPL-2.1-or-later
 * See CY_I2C_FRAM.h for design notes.
 */

#include "CY_I2C_FRAM.h"

namespace {
    constexpr uint32_t ADDR_WIDTH_1_LIMIT = 256UL; // <=256 Byte -> 1-byte word address
}

// ----------------------------------------------------------------------
// Function:    CY_I2C_FRAM (constructor)
// Arguments:   deviceAddress - 7-bit I2C address; capacityBytes - device
//              capacity; clockHz - I2C clock; chunkBytes - max payload
//              per Wire transaction; wireBus - TwoWire instance to use
// Return:      -
// Purpose:     Stores configuration and derives the address width (1 or
//              2 bytes) from the declared capacity. Does not touch
//              hardware; see begin().
// ----------------------------------------------------------------------
CY_I2C_FRAM::CY_I2C_FRAM(uint8_t deviceAddress, uint32_t capacityBytes,
                          uint32_t clockHz, size_t chunkBytes, TwoWire &wireBus)
    : _wire(wireBus),
      _deviceAddress(deviceAddress),
      _capacity(capacityBytes),
      _clockHz(clockHz),
      _chunkBytes(chunkBytes),
      _addrBytes(capacityBytes > ADDR_WIDTH_1_LIMIT ? 2 : 1),
      _initialized(false)
{
}

// ----------------------------------------------------------------------
// Function:    begin
// Arguments:   -
// Return:      Status::OK
// Purpose:     Starts the I2C bus and sets the configured clock. Must
//              be called once before any other public method.
// ----------------------------------------------------------------------
CY_I2C_FRAM::Status CY_I2C_FRAM::begin()
{
    _wire.begin();
    _wire.setClock(_clockHz);
    _initialized = true;
    return Status::OK;
}

// ----------------------------------------------------------------------
// Function:    mapEndTransmission
// Arguments:   code - raw return value of Wire::endTransmission()
// Return:      corresponding Status value
// Purpose:     Translates the Wire library's numeric transmission
//              result into a named, documented Status so callers do
//              not need to memorize Wire's magic numbers.
// ----------------------------------------------------------------------
CY_I2C_FRAM::Status CY_I2C_FRAM::mapEndTransmission(uint8_t code) const
{
    switch (code) {
        case 0: {
            return Status::OK;
        }
        case 1: {
            return Status::ERR_I2C_DATA_TOO_LONG;
        }
        case 2: {
            return Status::ERR_I2C_NACK_ADDRESS;
        }
        case 3: {
            return Status::ERR_I2C_NACK_DATA;
        }
        case 4: {
            return Status::ERR_I2C_BUS_ERROR;
        }
        default: {
            return Status::ERR_I2C_TIMEOUT; // covers code 5 and any future codes
        }
    }
}

// ----------------------------------------------------------------------
// Function:    writeAddress
// Arguments:   address - target F-RAM word address
// Return:      -
// Purpose:     Clocks out the word address in the device's configured
//              width (1 or 2 bytes, MSB first if 2 bytes). Internal
//              helper only; must be called between beginTransmission()
//              and endTransmission().
// ----------------------------------------------------------------------
void CY_I2C_FRAM::writeAddress(uint32_t address)
{
    if (_addrBytes == 2) {
        _wire.write((uint8_t)((address >> 8) & 0xFF));
    }
    _wire.write((uint8_t)(address & 0xFF));
}

// ----------------------------------------------------------------------
// Function:    checkRange
// Arguments:   address - start address; length - number of bytes
// Return:      Status::OK, ERR_ZERO_LENGTH or ERR_ADDRESS_OUT_OF_RANGE
// Purpose:     Overflow-safe bounds check against the declared device
//              capacity. Called by every public read/write method
//              before any bytes are put on the bus.
// ----------------------------------------------------------------------
CY_I2C_FRAM::Status CY_I2C_FRAM::checkRange(uint32_t address, size_t length) const
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
// Function:    pingDevice
// Arguments:   -
// Return:      Status::OK if the device ACKs, else a mapped I2C error
// Purpose:     Zero-length-write presence/self-test. I2C F-RAM parts do
//              not expose a generic SPI-style RDID opcode, so this is
//              the closest equivalent "is a chip here" check.
// ----------------------------------------------------------------------
CY_I2C_FRAM::Status CY_I2C_FRAM::pingDevice()
{
    if (!_initialized) {
        return Status::ERR_NOT_INITIALIZED;
    }
    _wire.beginTransmission(_deviceAddress);
    uint8_t code = _wire.endTransmission();
    return mapEndTransmission(code);
}

// ----------------------------------------------------------------------
// Function:    writeChunk
// Arguments:   address - start address of this chunk; buffer - source
//              data; length - byte count (<= _chunkBytes payload)
// Return:      mapped Status from Wire::endTransmission()
// Purpose:     Performs one addressed I2C write not exceeding the
//              configured chunk size. Internal helper used by
//              writeByte()/writeBurst().
// ----------------------------------------------------------------------
CY_I2C_FRAM::Status CY_I2C_FRAM::writeChunk(uint32_t address, const uint8_t *buffer, size_t length)
{
    _wire.beginTransmission(_deviceAddress);
    writeAddress(address);
    for (size_t i = 0; i < length; ++i) {
        _wire.write(buffer[i]);
    }
    uint8_t code = _wire.endTransmission(true); // STOP after each chunk
    return mapEndTransmission(code);
}

// ----------------------------------------------------------------------
// Function:    readChunk
// Arguments:   address - start address of this chunk; buffer -
//              destination; length - byte count (<= _chunkBytes)
// Return:      Status::OK, a mapped Wire error, or ERR_I2C_READ_SHORT
// Purpose:     Performs one addressed I2C read (repeated START, then
//              requestFrom) not exceeding the configured chunk size.
//              Internal helper used by readByte()/readBurst().
// ----------------------------------------------------------------------
CY_I2C_FRAM::Status CY_I2C_FRAM::readChunk(uint32_t address, uint8_t *buffer, size_t length)
{
    _wire.beginTransmission(_deviceAddress);
    writeAddress(address);
    uint8_t code = _wire.endTransmission(false); // repeated START, keep bus
    Status s = mapEndTransmission(code);
    if (s != Status::OK) {
        return s;
    }

    size_t received = (size_t)_wire.requestFrom((int)_deviceAddress, (int)length, (int)true);
    if (received < length) {
        return Status::ERR_I2C_READ_SHORT;
    }

    for (size_t i = 0; i < length; ++i) {
        buffer[i] = _wire.read();
    }
    return Status::OK;
}

// ----------------------------------------------------------------------
// Function:    writeByte
// Arguments:   address - target address; data - byte to write;
//              verify - if true, reads back and compares
// Return:      Status::OK or an error code (see CY_I2C_FRAM.h)
// Purpose:     Writes a single byte, with optional read-back
//              verification.
// ----------------------------------------------------------------------
CY_I2C_FRAM::Status CY_I2C_FRAM::writeByte(uint32_t address, uint8_t data, bool verify)
{
    if (!_initialized) {
        return Status::ERR_NOT_INITIALIZED;
    }
    Status rangeStatus = checkRange(address, 1);
    if (rangeStatus != Status::OK) {
        return rangeStatus;
    }

    Status s = writeChunk(address, &data, 1);
    if (s != Status::OK) {
        return s;
    }

    if (verify) {
        uint8_t readBack = 0;
        s = readByte(address, readBack);
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
// Purpose:     Reads a single byte.
// ----------------------------------------------------------------------
CY_I2C_FRAM::Status CY_I2C_FRAM::readByte(uint32_t address, uint8_t &data)
{
    if (!_initialized) {
        return Status::ERR_NOT_INITIALIZED;
    }
    Status rangeStatus = checkRange(address, 1);
    if (rangeStatus != Status::OK) {
        return rangeStatus;
    }

    return readChunk(address, &data, 1);
}

// ----------------------------------------------------------------------
// Function:    writeBurst
// Arguments:   address - start address; buffer - source data;
//              length - byte count; verify - if true, reads back and
//              compares every byte
// Return:      Status::OK or an error code
// Purpose:     Writes a contiguous block, automatically split into
//              chunks that fit the configured Wire buffer budget.
//              Bounds- and NULL-checked before any bus activity.
// ----------------------------------------------------------------------
CY_I2C_FRAM::Status CY_I2C_FRAM::writeBurst(uint32_t address, const uint8_t *buffer,
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

    const size_t payloadPerChunk = (_chunkBytes > _addrBytes) ? (_chunkBytes - _addrBytes) : 1;

    size_t offset = 0;
    while (offset < length) {
        size_t n = length - offset;
        if (n > payloadPerChunk) {
            n = payloadPerChunk;
        }

        Status s = writeChunk(address + offset, buffer + offset, n);
        if (s != Status::OK) {
            return s;
        }

        offset += n;
    }

    if (verify) {
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
// Purpose:     Reads a contiguous block, automatically split into
//              chunks that fit the configured Wire buffer budget.
//              Bounds- and NULL-checked before any bus activity.
// ----------------------------------------------------------------------
CY_I2C_FRAM::Status CY_I2C_FRAM::readBurst(uint32_t address, uint8_t *buffer, size_t length)
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

    const size_t payloadPerChunk = (_chunkBytes > _addrBytes) ? (_chunkBytes - _addrBytes) : 1;

    size_t offset = 0;
    while (offset < length) {
        size_t n = length - offset;
        if (n > payloadPerChunk) {
            n = payloadPerChunk;
        }

        Status s = readChunk(address + offset, buffer + offset, n);
        if (s != Status::OK) {
            return s;
        }

        offset += n;
    }
    return Status::OK;
}
