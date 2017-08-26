/*
 * Copyright (c) 2014 by Cypress Semiconductor Technology Pvt Ltd. <medu@cypress.com>
 * SPI F-RAM header file for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(CYSPIFRAM_H)
#define CYSPIFRAM_H

#include <stdint.h>

// Arduino IO Assignment for SPI
#define SI      11 //MOSI
#define SO      12 //MISO
#define SCK     13 //sck
#define CS      10 //ss

// SPI F-RAM Opcodes
#define WREN  0x06   // Write Enable Opcode
#define WRDI  0x04   // Write Disable Opcode
#define RDSR  0x05   // Read Status Register Opcode
#define WRSR  0x01   // Write Status Register Opcode
#define READ  0x03   // Read Opcode
#define WRITE 0x02   // Write Opcode

// address masks
#define ADDR_MSB_MASK   (uint16_t)0xFF00
#define ADDR_LSB_MASK   (uint16_t)0x00FF
#define MSB_ADDR_BYTE(addr)   ((uint8_t)((addr & ADDR_MSB_MASK)>>8))
#define LSB_ADDR_BYTE(addr)   ((uint8_t)(addr & ADDR_LSB_MASK))

#define NULL_PTR         ((uint8_t*) 0u)

// API decaration

// SPI F-RAM Initialization
extern void  FRAM_SPI_Init  ( void  );

// F-RAM Write function
extern void  FRAM_SPI_Write ( uint32_t , uint8_t );

// F-RAM read function
extern uint8_t  FRAM_SPI_Read  ( uint32_t );

// F-RAM BurstWrite function
extern void  FRAM_SPI_BurstWrite ( uint32_t , uint8_t * , uint32_t );

// F-RAM BurstRead function
extern void  FRAM_SPI_BurstRead  ( uint32_t , uint8_t * , uint32_t );

// Status register write function
extern void  FRAM_SPI_Status_Reg_Write( uint8_t  );

// Status register read function
extern uint8_t  FRAM_SPI_Status_Reg_Read ( void   );

#endif // End of CYSPIFRAM_H

#ifdef __cplusplus
}
#endif

//[] END OF FILE
