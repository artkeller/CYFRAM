# CY_SPI_FRAM – Pro-Rewrite: Änderungsprotokoll

Ausgangsbasis: `src/CYSPIFRAM.cpp` (Original © 2014 Cypress Semiconductor,
Arduino-Port 2017 artkeller@gmx.de).

| # | Schwäche im Original | Fix in der Pro-Version |
|---|---|---|
| 1 | Kein `SPI.beginTransaction()`/`endTransaction()` | Jede öffentliche Operation kapselt sich vollständig in einer Transaction (inkl. WREN-Subschritt) – bus- und interruptsicher auf geteilten SPI-Bussen. |
| 2 | Kein Interrupt-/Bus-Schutz während CS-Low-Phase | Durch die Transaction-Klammer implizit gelöst (Arduino-SPI-Kern sperrt/entsperrt den Bus selbst). |
| 3 | Globaler Zustand, ein fest verdrahteter CS-Pin, keine Mehrfachinstanzen | `CY_SPI_FRAM` ist eine Klasse; pro Chip/CS-Pin eine Instanz, beliebig viele F-RAM-Bausteine an einem Bus. |
| 4 | Adressraum hart auf 2 Byte begrenzt, trotz `uint32_t`-Parameter | Adressbreite (2 oder 3 Byte) wird automatisch aus der im Konstruktor angegebenen Kapazität abgeleitet. |
| 5 | Keine Rückgabewerte/Fehlerbehandlung, kein Verify | `Status`-Enum als Rückgabewert jeder Operation; optionales Read-Back-Verify nach Schreiboperationen. |
| 6 | `total_count` ungebounded, kein NULL-Check bei Single-Byte-Ops | Zentrale `checkRange()`-Prüfung (überlaufsicher) vor jedem Zugriff; NULL-Prüfung auch bei Burst-Buffern. |
| 7 | Keine Möglichkeit, Chip-Präsenz zu verifizieren | `readDeviceID()` (RDID, 0x9F) als Selbsttest ergänzt; `writeDisable()` (WRDI) für defensive Nutzung ergänzt. |
| 8 | Fester SPI-Takt (was auch immer `SPI.begin()` hinterlässt) | Konfigurierbarer Takt über `SPISettings` im Konstruktor (Default 20 MHz, `SPI_MODE0`). |

## Bewusst nicht geändert
- Die unterstützten Opcodes (WREN, WRDI, RDSR, WRSR, READ, WRITE, RDID)
  entsprechen weiterhin dem Standard-Cypress/Infineon-F-RAM-Befehlssatz.
- Lizenzmodell (GPLv2/LGPLv2.1 dual) aus dem Originalcode übernommen.

## I2C-Variante (CY_I2C_FRAM)

Gleiches Umbauprinzip wie bei `CY_SPI_FRAM`, übertragen auf I2C (FM24-artige
Cypress/Infineon-Teile). Zusätzlich zu den obigen acht Punkten löst die
I2C-Variante ein I2C-spezifisches Problem, das im ursprünglichen Repo laut
README zwar als Feature genannt, aber im gesichteten Code nicht mit
Absicherung versehen war:

| # | Schwäche (I2C-spezifisch) | Fix |
|---|---|---|
| 9 | Kein Chunking bei Burst-Transfers – die Wire-Bibliothek hat einen kleinen, plattformabhängigen internen Puffer (klassisch 32 Byte auf AVR); ein einzelner `Wire.write()`-Block über die Pufferlänge hinaus wird stillschweigend abgeschnitten statt einen Fehler zu liefern. | `writeBurst()`/`readBurst()` zerlegen automatisch in puffergerechte Chunks (`chunkBytes`-Parameter, Default 30 Byte inkl. Reserve für die Adressbytes). |
| 10 | Keine Unterscheidung der I2C-Fehlerursachen (Adress-NACK = kein Chip vorhanden vs. Daten-NACK vs. Bus-Fehler) | `Wire::endTransmission()`-Rückgabecodes werden 1:1 auf sprechende `Status`-Werte gemappt (`ERR_I2C_NACK_ADDRESS`, `ERR_I2C_NACK_DATA`, `ERR_I2C_BUS_ERROR`, `ERR_I2C_TIMEOUT`). |
| 11 | Kein Äquivalent zum SPI-Chip-Selbsttest (RDID existiert bei I2C-F-RAM nicht generisch) | `pingDevice()` als Zero-Length-Write-Präsenztest ergänzt. |

## Bekannte Grenzen der Pro-Version
- `readDeviceID()` liefert bei Chips ohne RDID-Unterstützung plausible,
  aber bedeutungslose Bytes (Bus-Leerlaufwert) – die Bewertung des
  Ergebnisses (z. B. "alles 0x00/0xFF -> kein Chip") liegt beim Aufrufer,
  da das Verhalten je nach Fremd-Chip variiert.
- `writeBurst(..., verify=true)` liest byteweise zurück (kein Zusatz-RAM
  nötig), ist dadurch aber langsamer als ein Blockvergleich. Für sehr
  große Burst-Writes ggf. bewusst ohne Verify aufrufen und stattdessen
  gezielt stichprobenartig verifizieren.
- Keine automatische Wiederholung bei `ERR_VERIFY_FAILED` – Retry-Strategie
  bewusst dem Aufrufer überlassen (anwendungsabhängig).
- `CY_I2C_FRAM` deckt keine Teile ab, die zusätzliche Adressbits über die
  7-Bit-Geräteadresse selbst kodieren (vergleichbar 24-Serie-EEPROM
  Block-Select) – bei sehr großen I2C-F-RAM-Varianten Datenblatt prüfen.
- Der `chunkBytes`-Default (30) ist konservativ für klassische AVR-Wire-
  Implementierungen gewählt; auf Plattformen mit größerem Wire-Puffer
  (z. B. ESP32) kann er im Konstruktor erhöht werden, um weniger
  I2C-Transaktionen pro Burst zu erzeugen.
