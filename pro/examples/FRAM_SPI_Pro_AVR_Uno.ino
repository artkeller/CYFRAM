/*
 * FRAM_SPI_Pro_AVR_Uno.ino
 *
 * Board target: classic AVR boards (Uno, Nano, Mega) with a hardware
 * SPI bus fixed to specific pins (Uno/Nano: MOSI=11, MISO=12, SCK=13).
 * Only the Chip-Select pin is freely selectable.
 */

#include <SPI.h>
#include "CY_SPI_FRAM.h"

// AVR hardware SPI pins are fixed by the core; only CS is configurable.
constexpr uint8_t  FRAM_CS_PIN   = 10;
constexpr uint32_t FRAM_CAPACITY = 32768UL;   // 256 Kbit part -> 32 KiB
// Conservative clock for breadboard wiring on AVR; raise if your wiring
// is short/clean (Cypress/Infineon F-RAM SPI parts commonly go up to
// 20-40 MHz depending on exact part -- check your datasheet).
constexpr uint32_t FRAM_CLOCK_HZ = 8000000UL;

CY_SPI_FRAM fram(FRAM_CS_PIN, FRAM_CAPACITY, FRAM_CLOCK_HZ); // uses default SPI bus

void setup()
{
    Serial.begin(115200);
    while (!Serial) { }

    Serial.println(F("\nF-RAM SPI Pro Example (AVR)"));

    if (fram.begin() != CY_SPI_FRAM::Status::OK) {
        Serial.println(F("begin() failed"));
    }

    uint8_t id[4] = {0};
    fram.readDeviceID(id, sizeof(id));
    Serial.print(F("Device ID: "));
    for (uint8_t b : id) { Serial.print(b, HEX); Serial.print(' '); }
    Serial.println();

    CY_SPI_FRAM::Status s = fram.writeByte(0x0000, 0xA5, /*verify=*/true);
    Serial.print(F("writeByte verify: "));
    Serial.println(s == CY_SPI_FRAM::Status::OK ? F("OK") : F("FAILED"));

    uint8_t value = 0;
    fram.readByte(0x0000, value);
    Serial.print(F("Read back: 0x"));
    Serial.println(value, HEX);
}

void loop() {}
