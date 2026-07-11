/*
 * FRAM_I2C_Pro_ESP32.ino
 *
 * Board target: ESP32 (Arduino-ESP32 core). ESP32's Wire implementation
 * typically tolerates larger transactions than classic AVR's 32-byte
 * buffer, so this example raises chunkBytes -- verify the safe maximum
 * for your specific core version rather than assuming a fixed number,
 * since it has varied across Arduino-ESP32 core releases.
 */

#include <Wire.h>
#include "CY_I2C_FRAM.h"

// Freely chosen GPIOs for this example -- adjust to your wiring.
constexpr uint8_t FRAM_SDA_PIN = 21;
constexpr uint8_t FRAM_SCL_PIN = 22;

constexpr uint8_t  FRAM_I2C_ADDRESS = 0x50;
constexpr uint32_t FRAM_CAPACITY    = 32768UL;   // 256 Kbit part -> 32 KiB
constexpr uint32_t FRAM_CLOCK_HZ    = 400000UL;  // raise to 1 MHz only if your
                                                  // specific part/wiring supports it
constexpr size_t   FRAM_CHUNK_BYTES = 60;        // conservative bump vs. AVR default;
                                                  // increase further only after testing

CY_I2C_FRAM fram(FRAM_I2C_ADDRESS, FRAM_CAPACITY, FRAM_CLOCK_HZ, FRAM_CHUNK_BYTES);

void setup()
{
    Serial.begin(115200);
    delay(200);

    Serial.println("\nF-RAM I2C Pro Example (ESP32)");

    Wire.begin(FRAM_SDA_PIN, FRAM_SCL_PIN); // custom pins before fram.begin()
    if (fram.begin() != CY_I2C_FRAM::Status::OK) {
        Serial.println("begin() failed");
    }

    CY_I2C_FRAM::Status s = fram.pingDevice();
    Serial.printf("pingDevice: %s\n", s == CY_I2C_FRAM::Status::OK ? "OK (chip present)" : "FAILED (check wiring/address)");

    s = fram.writeByte(0x0000, 0xA5, /*verify=*/true);
    Serial.printf("writeByte verify: %s\n", s == CY_I2C_FRAM::Status::OK ? "OK" : "FAILED");

    uint8_t value = 0;
    fram.readByte(0x0000, value);
    Serial.printf("Read back: 0x%02X\n", value);

    uint8_t writeBuf[128];
    for (uint16_t i = 0; i < sizeof(writeBuf); ++i) writeBuf[i] = (uint8_t)i;
    s = fram.writeBurst(0x1000, writeBuf, sizeof(writeBuf), /*verify=*/true);
    Serial.printf("writeBurst(128B) verify: %s\n", s == CY_I2C_FRAM::Status::OK ? "OK" : "FAILED");
}

void loop() {}
