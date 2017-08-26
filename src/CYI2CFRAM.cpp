/*
 * Copyright (c) 2014 by Cypress Semiconductor Technology Pvt Ltd. <medu@cypress.com>
 * I2C F-RAM Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include <Wire.h>
#include "CYI2CFRAM.h"

//****************************************************************************************
// Function   : FRAM_I2C_Init
// Arguments  : None
// Return     : None
// Purpose    : Initialization routine for FRAM I2C
// Usage      : FRAM_I2C_Init();
//            : This function is called at the
//            : beginning of program along with other initializations.
//****************************************************************************************
void FRAM_I2C_Init ( void )
{
    // Initialize I2C
    Wire.begin();
}

//****************************************************************************************
// Function   : FRAM_I2C_Write
// Arguments  : uint8_t  slave_id         -> 7 bit Slave ID
//            : uint32_t addr             -> 32 bit FRAM address for write.
//            : uint8_t *data_write_ptr   -> Pointer to an array of data bytes to be written.
//            : uint32_t total_data_count -> Number of data bytes to be written.
// Return     : uint32_t                   -> error status
//            :    Success                                   -- (0x00u)
//            :    Reserved                                  -- (0x01u)
//            :    Received NACK on transmit of address      -- (0x02u)
//            :    Received NACK on transmit of data         -- (0x03u)
//            :    Other error                               -- (0x04u)
//
// Purpose    : I2C FRAM Write Function
// Usage      : FRAM_I2C_Write(slave_id, addr, data_bytes, 8);
//****************************************************************************************
uint32_t FRAM_I2C_Write (uint8_t slave_id, uint32_t addr, uint8_t *data_write_ptr, uint32_t total_data_count )
{
    uint32_t i;
    uint32_t error_status = 0;
    uint8_t t_cnt;
    uint32_t data_index;

    // If write pointer is NULL, return error
    if(data_write_ptr == NULL_PTR)
        return FRAM_I2C_MSTR_ERROR;

    // Initialize the data index
    data_index = 0;

    // Wire library can transmit a maximum of I2C_BUF_LEN(32 bytes) at a time
    while(total_data_count > 0)
    {
        // Get the number of byte to transmit
        t_cnt = (total_data_count > (I2C_BUF_LEN - 2))? (I2C_BUF_LEN - 2) : total_data_count;

        Wire.beginTransmission(slave_id);       // transmit to slave_id
        Wire.write(MSB_ADDR_BYTE(addr));        // sends MSB address byte
        Wire.write(LSB_ADDR_BYTE(addr));        // sends LSB address byte

        Wire.write(&data_write_ptr[data_index], t_cnt);  // send data bytes

        error_status = Wire.endTransmission(true);   // stop transmitting

        total_data_count -= t_cnt;    // This contains the remaining number of bytes to transmit
        addr += t_cnt;                // Increment the address by the number of bytes transmitted
        data_index += t_cnt;

        // If the transmission not success, exit
        if(error_status != FRAM_I2C_MSTR_NO_ERROR)
        {
            break;
        }
    }

    // Return the error status
    return error_status;
}

//****************************************************************************************
// Function   : FRAM_I2C_Current_Read
// Arguments  : uint8_t  slave_id         -> 7 bit Slave ID
//            : uint8_t *data_read_ptr    -> Pointer to an array for storing data bytes.
//            : uint32_t total_data_count -> Number of data bytes to be read.
// Return     : uint32_t                  -> Number of bytes received
// Usage      : FRAM_I2C_Current_Read();
//            : Eg: FRAM_I2C_Current_Read(slave_id, addr, data_bytes, 8);
//****************************************************************************************
uint32_t FRAM_I2C_Current_Read ( uint8_t slave_id, uint8_t *data_read_ptr, uint32_t total_data_count )
{
   uint8_t no_of_bytes_read = 0;
   uint32_t i = 0;
   uint32_t j = 0;
   uint32_t total_no_of_bytes_read = 0;
   uint32_t t_cnt;

   // If read pointer is NULL, return 0 number of bytes
   if(data_read_ptr == NULL_PTR)
       return ZERO_BYTES;

   // Wire library can receive a maximumum of I2C_BUF_LEN(32 bytes) at a time
   while(total_data_count > 0)
   {
      // request total_data_count bytes from slave device
      // Internal buffer is 32 bytes
      t_cnt = (total_data_count > I2C_BUF_LEN)? I2C_BUF_LEN : total_data_count;
      no_of_bytes_read = Wire.requestFrom((int)slave_id, t_cnt, true);
      
      // If the number of bytes read is 0, return
      if(no_of_bytes_read == 0)
          return ZERO_BYTES;

      for(j = 0; j < no_of_bytes_read; j++)
      {
         data_read_ptr[i++] = Wire.read(); // receive a byte
      }

      total_data_count -= no_of_bytes_read;
      total_no_of_bytes_read += no_of_bytes_read;
   }

   return total_no_of_bytes_read;
}
//****************************************************************************************
// Function   : FRAM_I2C_Random_Read
// Arguments  : uint8_t  slave_id         -> 7 bit Slave ID
//            : uint32_t addr             -> 32 bit FRAM address for read.
//            : uint8_t *data_read_ptr    -> Pointer to an array for storing data bytes.
//            : uint32_t total_data_count -> Number of data bytes to be read.
// Return     : uint32_t                  -> Number of bytes received
// Purpose    : I2C FRAM Random Read Function
// Usage      : FRAM_I2C_Random_Read();
//            : Eg: FRAM_I2C_Random_Read(slave_id, addr, data_bytes, 8);
//****************************************************************************************
uint32_t FRAM_I2C_Random_Read ( uint8_t slave_id, uint32_t addr, uint8_t *data_read_ptr, uint32_t total_data_count )
{
    uint8_t no_of_bytes_read = 0;
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t total_no_of_bytes_read = 0;
    uint32_t t_cnt;
    uint32_t error_status = 0;

    // If read pointer is NULL, return 0 number of bytes
    if(data_read_ptr == NULL_PTR)
        return ZERO_BYTES;

    Wire.beginTransmission(slave_id);      // transmit to slave_id
    Wire.write(MSB_ADDR_BYTE(addr));       // sends MSB address byte
    Wire.write(LSB_ADDR_BYTE(addr));       // sends LSB address byte
    error_status = Wire.endTransmission(true);    // stop transmitting

    // If the transmission not success, exit with 0 bytes
    if(error_status != FRAM_I2C_MSTR_NO_ERROR)
    {
        return ZERO_BYTES;
    }
        
    // Wire library can receive a maximumum of I2C_BUF_LEN(32 bytes) at a time
    while(total_data_count > 0)
    {
        // request total_data_count bytes from slave device
        // Internal buffer is 32 bytes
        t_cnt = (total_data_count > I2C_BUF_LEN)? I2C_BUF_LEN : total_data_count;
        no_of_bytes_read = Wire.requestFrom((int)slave_id, t_cnt, true);

        // If the number of bytes read is 0, return
        if(no_of_bytes_read == 0)
            return ZERO_BYTES;
          
        for(j = 0; j < no_of_bytes_read; j++)
        {
            data_read_ptr[i++] = Wire.read(); // receive a byte
        }

        total_data_count -= no_of_bytes_read;
        total_no_of_bytes_read += no_of_bytes_read;
    }

    return total_no_of_bytes_read;
}
