/*
 * FRAM_SPI_Pro_ESP32.ino
 *
 * Board target: ESP32 (Arduino-ESP32 core), which supports a second,
 * freely pin-mappable SPI bus (HSPI/VSPI depending on core version)
 * in addition to the default bus. This example wires up a dedicated
 * SPI bus instance instead of relying on default pins, and shows how
 * to hand that instance into CY_SPI_FRAM's constructor.
 */

#include <SPI.h>
#include "CY_SPI_FRAM.h"

// Freely chosen GPIOs for this example -- adjust to your wiring.
// (Any GPIO capable of general-purpose output/input works for
// software-mapped SPI on ESP32; avoid strapping pins.)
constexpr uint8_t FRAM_SCK_PIN  = 18;
constexpr uint8_t FRAM_MISO_PIN = 19;
constexpr uint8_t FRAM_MOSI_PIN = 23;
constexpr uint8_t FRAM_CS_PIN   = 5;

constexpr uint32_t FRAM_CAPACITY = 32768UL;   // 256 Kbit part -> 32 KiB
constexpr uint32_t FRAM_CLOCK_HZ = 20000000UL; // ESP32 handles this comfortably

// Dedicated SPI bus instance (do not reuse the default `SPI` object if
// other peripherals depend on its default pin mapping).
SPIClass framSPI(HSPI);

CY_SPI_FRAM fram(FRAM_CS_PIN, FRAM_CAPACITY, FRAM_CLOCK_HZ, framSPI);

void setup()
{
    Serial.begin(115200);
    delay(200); // ESP32 USB-CDC/native USB settle time

    Serial.println("\nF-RAM SPI Pro Example (ESP32)");

    framSPI.begin(FRAM_SCK_PIN, FRAM_MISO_PIN, FRAM_MOSI_PIN, FRAM_CS_PIN);

    if (fram.begin() != CY_SPI_FRAM::Status::OK) {
        Serial.println("begin() failed");
    }

    uint8_t id[4] = {0};
    fram.readDeviceID(id, sizeof(id));
    Serial.print("Device ID: ");
    for (uint8_t b : id) { Serial.printf("%02X ", b); }
    Serial.println();

    CY_SPI_FRAM::Status s = fram.writeByte(0x0000, 0xA5, /*verify=*/true);
    Serial.printf("writeByte verify: %s\n", s == CY_SPI_FRAM::Status::OK ? "OK" : "FAILED");

    uint8_t value = 0;
    fram.readByte(0x0000, value);
    Serial.printf("Read back: 0x%02X\n", value);
}

void loop() {}
