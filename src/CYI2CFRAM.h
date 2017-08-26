/*
 * Copyright (c) 2014 by Cypress Semiconductor Technology Pvt Ltd. <medu@cypress.com>
 * I2C F-RAM header file for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */
#ifdef __cplusplus
extern "C" {
#endif

#if !defined(CYI2CFRAM_H)
#define CYI2CFRAM_H

#include <stdint.h>

// Length of I2C Internal Buffer
#define I2C_BUF_LEN (32u)

// Errors

// No Error
#define FRAM_I2C_MSTR_NO_ERROR      (0x00u)

// Data buffer error. Data exceeds 32 bytes
#define FRAM_I2C_MSTR_DATABUF_ERROR (0x01u)

// Slave Address is NACKed
#define FRAM_I2C_MSTR_ADDR_NACK     (0x02u)

// Data is NACKed
#define FRAM_I2C_MSTR_DATA_NACK     (0x03u)

// Other Errors
#define FRAM_I2C_MSTR_ERROR         (0x04u)

// Null pointer
#define NULL_PTR                    ((uint8_t*) 0u)

// Zero bytes received
#define ZERO_BYTES                  (0u)

// address masks
#define ADDR_MSB_MASK   (uint16_t)0xFF00
#define ADDR_LSB_MASK   (uint16_t)0x00FF
#define MSB_ADDR_BYTE(addr)   ((uint8_t)((addr & ADDR_MSB_MASK)>>8))
#define LSB_ADDR_BYTE(addr)   ((uint8_t)(addr & ADDR_LSB_MASK))

// API decaration

// Initialization Function
extern void FRAM_I2C_Init  ( void  );

// I2C Write Function
extern uint32_t FRAM_I2C_Write ( uint8_t, uint32_t, uint8_t*, uint32_t );

// I2C Current Read Function
extern uint32_t  FRAM_I2C_Current_Read ( uint8_t, uint8_t *, uint32_t );

// I2C Random Read Function
extern uint32_t  FRAM_I2C_Random_Read ( uint8_t, uint32_t , uint8_t * , uint32_t );

#endif // End of CYI2CFRAM_H

#ifdef __cplusplus
}
#endif

//[] END OF FILE
