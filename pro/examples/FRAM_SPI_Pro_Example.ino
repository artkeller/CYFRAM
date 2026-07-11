/*
 * FRAM_SPI_Pro_Example.ino
 *
 * Demonstrates the object-oriented CY_SPI_FRAM API, including bounds
 * checking, write verification and device-ID self-test.
 */

#include <SPI.h>
#include "CY_SPI_FRAM.h"

// Adjust to your wiring / chip:
constexpr uint8_t  FRAM_CS_PIN   = 10;
constexpr uint32_t FRAM_CAPACITY = 32768UL;   // 256 Kbit part -> 32 KiB
constexpr uint32_t FRAM_CLOCK_HZ = 20000000UL;

CY_SPI_FRAM fram(FRAM_CS_PIN, FRAM_CAPACITY, FRAM_CLOCK_HZ);

void printStatus(const char *what, CY_SPI_FRAM::Status s)
{
    Serial.print(what);
    Serial.print(": ");
    switch (s) {
        case CY_SPI_FRAM::Status::OK:                      Serial.println("OK"); break;
        case CY_SPI_FRAM::Status::ERR_NOT_INITIALIZED:      Serial.println("ERROR not initialized"); break;
        case CY_SPI_FRAM::Status::ERR_NULL_POINTER:         Serial.println("ERROR null pointer"); break;
        case CY_SPI_FRAM::Status::ERR_ZERO_LENGTH:          Serial.println("ERROR zero length"); break;
        case CY_SPI_FRAM::Status::ERR_ADDRESS_OUT_OF_RANGE: Serial.println("ERROR address out of range"); break;
        case CY_SPI_FRAM::Status::ERR_VERIFY_FAILED:        Serial.println("ERROR verify failed"); break;
        case CY_SPI_FRAM::Status::ERR_UNSUPPORTED:          Serial.println("ERROR unsupported"); break;
    }
}

void setup()
{
    Serial.begin(115200);
    while (!Serial) { /* wait for USB CDC on some boards */ }

    Serial.println("\nF-RAM SPI Pro Example");
    Serial.println("----------------------");

    printStatus("begin()", fram.begin());

    // Self-test: is the chip actually there?
    uint8_t id[4] = {0};
    printStatus("readDeviceID()", fram.readDeviceID(id, sizeof(id)));
    Serial.print("Device ID: ");
    for (uint8_t b : id) { Serial.print(b, HEX); Serial.print(' '); }
    Serial.println();

    // Single byte write with verification
    printStatus("writeByte(0x0000, 0xA5, verify=true)",
                fram.writeByte(0x0000, 0xA5, true));

    uint8_t value = 0;
    printStatus("readByte(0x0000)", fram.readByte(0x0000, value));
    Serial.print("Read back: 0x"); Serial.println(value, HEX);

    // Burst write/read with verification
    uint8_t writeBuf[16];
    for (uint8_t i = 0; i < sizeof(writeBuf); ++i) writeBuf[i] = i;
    printStatus("writeBurst(0x1000, 16 bytes, verify=true)",
                fram.writeBurst(0x1000, writeBuf, sizeof(writeBuf), true));

    uint8_t readBuf[16] = {0};
    printStatus("readBurst(0x1000, 16 bytes)",
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
