# CYFRAM

[![CRA](https://img.shields.io/badge/CRA-Voluntary%20Full%20Compliance-informational?style=flat-square)](./CRA.md)
[![Language](https://img.shields.io/badge/language-🇬🇧%20English-informational)](./LANGUAGE.md)
[![License](https://img.shields.io/badge/License-GPL--2.0-blue?style=flat-square)](./LICENSE.md)
[![Version](https://img.shields.io/badge/Version-1.2.0-brightgreen?style=flat-square)](./CHANGELOG.md)
[![Security](https://img.shields.io/badge/Security-Policy-brightgreen?style=flat-square)](./SECURITY.md)
[![Code Style](https://img.shields.io/badge/Code%20Style-Arduino-orange?style=flat-square)](./CODESTYLE.md)
[![Platform](https://img.shields.io/badge/Platform-I²C%20%26%20SPI-lightgrey?style=flat-square)](./README.md)


### Cypress F-RAM Master Library for Arduino

**F-RAMs** (*Ferroelectric RAMs*) are *nonvolatile RAMs* with very high endurance (exceeding from 10^12 to 10^14 up to 10^16 read/write cycles **per byte**. F-RAMs store in byte-mode unlike block-mode of flash memories).

This library supports **I2C and SPI APIs** provided with the development kit *CY15FRAMKIT-001*. They where assembled as a lean all-in-one Arduino library to support generic Cypress F-RAM memories.

- The **I2C APIs** include initialization of block, write, current read and random read

- The **SPI APIs** include initialization of block, write/read memory, status register

Arduino standard libraries *Wire* and *SPI*  will be used for best MCU HW support.

Chip specific functions like vendor-IDs, sleep-mode, etc. or pin-handling for /WP and /HOLD are not supported in this version. 


## Examples 

Two sketches within the *CY15FRAMKIT-001 Kit Setup* had been modified and analysed with a logic analyzer. 

- **FRAM_I2C_Example_2.ino:** This sketch was used to test **FM24W256, 256-Kbit (32K x 8) Serial (I2C) F-RAM**

- **FRAM_SPI_Example_2.ino:** This sketch was used to test **FM25W256, 256-Kbit (32K x 8) Serial (SPI) F-RAM**

All test results are part of this distribution and reside in the folders **logic** and **output**. 

Although other Cypress F-RAMs are not tested yet memories with 2-byte addressing should work. Grateful if you can send results of tested F-RAM memories (of different vendors if feasable).

Larger F-RAM memories with 3-byte adressing are not supported until yet.

## UPDATE: Infineon aquired Cypress

2019: Infineon has announced the acquisition of Cypress Semiconductors in order to push further into the automotive, industrial, and Internet of Things (IoT) hardware markets.

2020: Infineon Technologies AG announced the Closing of the acquisition of Cypress Semiconductor Corporation. The San José-based company has become part of Infineon effective as of the Closing.

## More

### Valid original links 

- *Ferroelectric RAM:* https://en.wikipedia.org/wiki/Ferroelectric_RAM

- *nonvolatile RAMs (NVRAM):* https://en.wikipedia.org/wiki/Non-volatile_memory

- *Infineon and Cypress: Strengthening the link between the real and the digital world, September 2019*
https://www.infineon.com/dgdl/2019-09-16+Acquisition+of+Cypress.pdf?fileId=5546d4616b056b50016b1ba6930f0000

- *Infineon Technologies AG completes acquisition of Cypress Semiconductor Corporation):*
https://www.infineon.com/cms/en/about-infineon/press/press-releases/2020/INFXX202004-049.html

- *F-RAM (Nonvolatile Ferroelectric RAM):* https://www.infineon.com/cms/en/product/memories/f-ram-ferroelectric-ram/ 

- *CY15FRAMKIT-001:* 
https://www.infineon.com/cms/en/product/evaluation-boards/cy15framkit-001/

### Invalid original link - not directly supported by Infineon, but somehow forwarded

- *CY15FRAMKIT-001 Kit Setup (CY15FRAMKIT-001 Kit Setup - Kit Design Files,Documentation, Examples):*
(http://www.cypress.com/file/278596/download  - you must log in or register to access this page) 
-> *CE209976 – SPI Slave Select Inversion with Smart IO®:*
https://www.infineon.com/dgdl/Infineon-CE209976_SPI_Slave_Select_Inversion_with_Smart_I_O-Code+Example-v01_00-EN.pdf?fileId=8ac78c8c7cdc391c017d0d7f6b705028

#### Links mentioned above are valid since 2022-10-22

### Outdated original links - Supported by Infineon since 2020

- *F-RAM (Nonvolatile Ferroelectric RAM):*  
http://www.cypress.com/products/f-ram-nonvolatile-ferroelectric-ram

- *CY15FRAMKIT-001:* 
http://www.cypress.com/documentation/development-kitsboards/cy15framkit-001-serial-f-ram-development-kit-guide

#### Links mentioned above are valid since 2020


#### Comments are welcome
