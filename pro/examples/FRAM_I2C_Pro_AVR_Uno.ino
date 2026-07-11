/*
 * FRAM_I2C_Pro_AVR_Uno.ino
 *
 * Board target: classic AVR boards (Uno, Nano, Mega). The Wire library
 * on these cores classically has a small internal buffer (BUFFER_LENGTH,
 * commonly 32 Byte), which is exactly why CY_I2C_FRAM chunks bursts
 * automatically -- the default chunkBytes=30 is deliberately chosen to
 * stay safely under that limit.
 */

#include <Wire.h>
#include "CY_I2C_FRAM.h"

constexpr uint8_t  FRAM_I2C_ADDRESS = 0x50;      // adjust to address-pin strapping
constexpr uint32_t FRAM_CAPACITY    = 32768UL;   // 256 Kbit part -> 32 KiB
constexpr uint32_t FRAM_CLOCK_HZ    = 400000UL;  // Fast Mode

// Default chunkBytes=30 is appropriate for AVR's classic 32-byte Wire buffer.
CY_I2C_FRAM fram(FRAM_I2C_ADDRESS, FRAM_CAPACITY, FRAM_CLOCK_HZ);

void setup()
{
    Serial.begin(115200);
    while (!Serial) { }

    Serial.println(F("\nF-RAM I2C Pro Example (AVR)"));

    if (fram.begin() != CY_I2C_FRAM::Status::OK) {
        Serial.println(F("begin() failed"));
    }

    CY_I2C_FRAM::Status s = fram.pingDevice();
    Serial.print(F("pingDevice: "));
    Serial.println(s == CY_I2C_FRAM::Status::OK ? F("OK (chip present)") : F("FAILED (check wiring/address)"));

    s = fram.writeByte(0x0000, 0xA5, /*verify=*/true);
    Serial.print(F("writeByte verify: "));
    Serial.println(s == CY_I2C_FRAM::Status::OK ? F("OK") : F("FAILED"));

    uint8_t value = 0;
    fram.readByte(0x0000, value);
    Serial.print(F("Read back: 0x"));
    Serial.println(value, HEX);

    // Burst larger than one chunk to exercise the automatic chunking.
    uint8_t writeBuf[64];
    for (uint16_t i = 0; i < sizeof(writeBuf); ++i) writeBuf[i] = (uint8_t)i;
    s = fram.writeBurst(0x1000, writeBuf, sizeof(writeBuf), /*verify=*/true);
    Serial.print(F("writeBurst(64B) verify: "));
    Serial.println(s == CY_I2C_FRAM::Status::OK ? F("OK") : F("FAILED"));
}

void loop() {}
