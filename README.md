# CYFRAM
Cypress **F-RAM Master Library for Arduino**

The library supports **F-RAM** (*Ferroelectric RAM*) **I2C and SPI APIs** provided with the development kit *CY15FRAMKIT-001*. They where assembled as an Arduino library and support generic memory access.

- The **I2C APIs** include initialization of block, write, current read and random read

- The **SPI APIs** include initialization of block, write/read memory, status register

Memory pins /W and /HOLD are not supported in this version.


## Examples 

Two sketches within the *CY15FRAMKIT-001 Kit Setup* had been modified and analysed with a logic analyzer. 

- **FRAM_I2C_Example_2.ino:** This sketch was used to test **I2C F-RAM FM24W256**

- **FRAM_SPI_Example_2.ino:** This sketch was used to test **SPI F-RAM FM25W256**

All test results are part of this distribution and reside in the folders **logic** and **output**. Although other Cypress F-RAMs are not tested yet they should work until a size of 2MB (2 bytes address). Please send test result of other memories.

## More

- *Ferroelectric RAM:* https://en.wikipedia.org/wiki/Ferroelectric_RAM

- *CY15FRAMKIT-001:* 
http://www.cypress.com/documentation/development-kitsboards/cy15framkit-001-serial-f-ram-development-kit-guide

- *CY15FRAMKIT-001 Kit Setup (CY15FRAMKIT-001 Kit Setup - Kit Design Files,Documentation, Examples):*
http://www.cypress.com/file/278596/download

Links tested: 2017-08-27


<code> 
version=1.0.0
</code>
