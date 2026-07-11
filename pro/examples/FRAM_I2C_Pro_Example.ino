/*
 * FRAM_I2C_Pro_Example.ino
 *
 * Demonstrates the object-oriented CY_I2C_FRAM API, including bounds
 * checking, chunked bursts, write verification and a bus-presence
 * self-test.
 */

#include <Wire.h>
#include "CY_I2C_FRAM.h"

// Adjust to your wiring / chip:
constexpr uint8_t  FRAM_I2C_ADDRESS = 0x50;      // typical Cypress/Infineon base address
constexpr uint32_t FRAM_CAPACITY    = 32768UL;   // 256 Kbit part -> 32 KiB
constexpr uint32_t FRAM_CLOCK_HZ    = 400000UL;  // Fast Mode

CY_I2C_FRAM fram(FRAM_I2C_ADDRESS, FRAM_CAPACITY, FRAM_CLOCK_HZ);

void printStatus(const char *what, CY_I2C_FRAM::Status s)
{
    Serial.print(what);
    Serial.print(": ");
    switch (s) {
        case CY_I2C_FRAM::Status::OK:                      Serial.println("OK"); break;
        case CY_I2C_FRAM::Status::ERR_NOT_INITIALIZED:      Serial.println("ERROR not initialized"); break;
        case CY_I2C_FRAM::Status::ERR_NULL_POINTER:         Serial.println("ERROR null pointer"); break;
        case CY_I2C_FRAM::Status::ERR_ZERO_LENGTH:          Serial.println("ERROR zero length"); break;
        case CY_I2C_FRAM::Status::ERR_ADDRESS_OUT_OF_RANGE: Serial.println("ERROR address out of range"); break;
        case CY_I2C_FRAM::Status::ERR_VERIFY_FAILED:        Serial.println("ERROR verify failed"); break;
        case CY_I2C_FRAM::Status::ERR_I2C_DATA_TOO_LONG:    Serial.println("ERROR I2C data too long"); break;
        case CY_I2C_FRAM::Status::ERR_I2C_NACK_ADDRESS:     Serial.println("ERROR I2C NACK on address (no chip?)"); break;
        case CY_I2C_FRAM::Status::ERR_I2C_NACK_DATA:        Serial.println("ERROR I2C NACK on data"); break;
        case CY_I2C_FRAM::Status::ERR_I2C_BUS_ERROR:        Serial.println("ERROR I2C bus error"); break;
        case CY_I2C_FRAM::Status::ERR_I2C_TIMEOUT:          Serial.println("ERROR I2C timeout"); break;
        case CY_I2C_FRAM::Status::ERR_I2C_READ_SHORT:       Serial.println("ERROR I2C short read"); break;
    }
}

void setup()
{
    Serial.begin(115200);
    while (!Serial) { /* wait for USB CDC on some boards */ }

    Serial.println("\nF-RAM I2C Pro Example");
    Serial.println("-----------------------");

    printStatus("begin()", fram.begin());
    printStatus("pingDevice()", fram.pingDevice());

    // Single byte write with verification
    printStatus("writeByte(0x0000, 0xA5, verify=true)",
                fram.writeByte(0x0000, 0xA5, true));

    uint8_t value = 0;
    printStatus("readByte(0x0000)", fram.readByte(0x0000, value));
    Serial.print("Read back: 0x"); Serial.println(value, HEX);

    // Burst write/read larger than one Wire chunk (default chunk ~30 Byte)
    // to demonstrate automatic chunking.
    uint8_t writeBuf[64];
    for (uint16_t i = 0; i < sizeof(writeBuf); ++i) writeBuf[i] = (uint8_t)i;
    printStatus("writeBurst(0x1000, 64 bytes, verify=true)",
                fram.writeBurst(0x1000, writeBuf, sizeof(writeBuf), true));

    uint8_t readBuf[64] = {0};
    printStatus("readBurst(0x1000, 64 bytes)",
                fram.readBurst(0x1000, readBuf, sizeof(readBuf)));

    // Deliberately out-of-range access to show the new bounds checking:
    printStatus("writeByte(capacity(), ...) [expected: out of range]",
                fram.writeByte(fram.capacity(), 0x00));

    // Deliberately NULL-buffer access to show the new NULL check:
    printStatus("writeBurst(0x0, nullptr, 4) [expected: null pointer]",
                fram.writeBurst(0x0, nullptr, 4));
}

void loop()
{
    // Nothing to do here.
}
