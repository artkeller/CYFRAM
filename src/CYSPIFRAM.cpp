/*
 * Copyright (c) 2014 by Cypress Semiconductor Technology Pvt Ltd. <medu@cypress.com>
 * SPI F-RAM Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */
#include <SPI.h>
#include "CYSPIFRAM.h"

//--------------------------------------------------------------------------------------
// FRAM_SPI_Init
// Purpose   : API to initialize the SPI F-RAM
// Arguments : None
// Return    : None
// Usage     : Eg: FRAM_SPI_Init();
//--------------------------------------------------------------------------------------
void FRAM_SPI_Init()
{
    // Initialize SPI
    SPI.begin();

    // set the CS as an output:
    pinMode(CS, OUTPUT);

    digitalWrite(CS,HIGH); //Disable Chip
}

//--------------------------------------------------------------------------------------
// FRAM_SPI_Write
// Purpose   : API to write to SPI F-RAM
// Arguments : FRAM_address -> 2 byte F-RAM address
//             FRAM_data    -> 1 byte data
// Return    : None
// Usage     : Eg: FRAM_SPI_Write(0x0000, 0x55); Writes data byte 0x55 to F-RAM address 0x0000
//--------------------------------------------------------------------------------------
void FRAM_SPI_Write(uint32_t FRAM_address, uint8_t FRAM_data)
{
  // F-RAM WRITE ENABLE COMMAND
  digitalWrite(CS,LOW);  //chip select
  SPI.transfer(WREN);    //transmit write enable opcode
  digitalWrite(CS,HIGH); //release chip, signal end transfer

  // F-RAM WRITE OPERATION
  digitalWrite(CS,LOW);                      //chip select
  SPI.transfer(WRITE);                       //transmit write opcode
  SPI.transfer(MSB_ADDR_BYTE(FRAM_address));  //send MSByte address first
  SPI.transfer(LSB_ADDR_BYTE(FRAM_address));  //send LSByte address
  SPI.transfer((uint8_t)(FRAM_data));        //send 1 byte data
  digitalWrite(CS,HIGH);                     //release chip, signal end of transfer
}

//--------------------------------------------------------------------------------------
// FRAM_SPI_Read
// Purpose   : API to read from SPI F-RAM
// Arguments : FRAM_address -> 2 byte F-RAM address
// Return    : 1 byte data read from F-RAM
// Usage     : Eg: value = FRAM_SPI_Read(0x0000); Reads 1-byte data from F-RAM and assigns to
//           : variable 'value'
//--------------------------------------------------------------------------------------
uint8_t FRAM_SPI_Read(uint32_t FRAM_address)
{
  uint8_t data;

  //FRAM READ OPERATION

  digitalWrite(CS,LOW);                     //chip select
  SPI.transfer(READ);                       //transmit read opcode
  SPI.transfer(MSB_ADDR_BYTE(FRAM_address));  //send MSByte address first
  SPI.transfer(LSB_ADDR_BYTE(FRAM_address));  //send LSByte address
  data = SPI.transfer(0xFF);                //get data byte from F-RAM
  digitalWrite(CS,HIGH);                    //release chip, signal end of transfer

  return data;   // Return data byte
}

//--------------------------------------------------------------------------------------
// FRAM_SPI_BurstWrite
// Purpose   : API to write to SPI F-RAM
// Arguments : FRAM_address -> 2 byte F-RAM address
//           : FRAM_wr_data_ptr -> Data Buffer which needs to be written into FRAM
//           : total_count  -> Total number of bytes to write
// Return    : None
// Usage     : Eg: FRAM_SPI_BurstWrite(0x0000, wr_buf,16); Writes 16 bytes of data from wr_buf to F-RAM at address 0x0000
//--------------------------------------------------------------------------------------
void FRAM_SPI_BurstWrite(uint32_t FRAM_address, uint8_t * FRAM_wr_data_ptr, uint32_t total_count)
{
  uint32_t i;

  // If write pointer is NULL, return
  if(FRAM_wr_data_ptr == NULL_PTR)
      return;

  // F-RAM WRITE ENABLE COMMAND
  digitalWrite(CS,LOW);  //chip select
  SPI.transfer(WREN);    //transmit write enable opcode
  digitalWrite(CS,HIGH); //release chip, signal end transfer

  // F-RAM WRITE OPERATION
  digitalWrite(CS,LOW);                   //chip select
  SPI.transfer(WRITE);                    //transmit write opcode
  SPI.transfer(MSB_ADDR_BYTE(FRAM_address));  //send MSByte address first
  SPI.transfer(LSB_ADDR_BYTE(FRAM_address));  //send LSByte address

  // Data byte transmission
  for(i=0;i<total_count;i++)
     SPI.transfer((uint8_t)(FRAM_wr_data_ptr[i]));        //send 1 byte data at a time

  digitalWrite(CS,HIGH);                  //release chip, signal end of transfer
}

//--------------------------------------------------------------------------------------
// FRAM_SPI_BurstRead
// Purpose   : API to read from SPI F-RAM
// Arguments : FRAM_address -> 2 byte F-RAM address
//           : FRAM_rd_data_ptr -> Buffer to hold the data being read
//           : total_count  -> Total number of bytes to read
// Return    : None
// Usage     : Eg: value = FRAM_SPI_BurstRead(0x0000, rd_buf, 16); Reads 16-bytes of data from F-RAM starting
//           : at address 0x0000 and stores into rd_buf
//--------------------------------------------------------------------------------------
void FRAM_SPI_BurstRead(uint32_t FRAM_address, uint8_t * FRAM_rd_data_ptr, uint32_t total_count)
{
  uint32_t i;

  // If read pointer is NULL, return
  if(FRAM_rd_data_ptr == NULL_PTR)
      return;

  //FRAM READ OPERATION

  digitalWrite(CS,LOW);                     //chip select
  SPI.transfer(READ);                       //transmit read opcode
  SPI.transfer(MSB_ADDR_BYTE(FRAM_address));  //send MSByte address first
  SPI.transfer(LSB_ADDR_BYTE(FRAM_address));  //send LSByte address
  
  for(i=0;i<total_count;i++)
     FRAM_rd_data_ptr[i] = SPI.transfer(0xFF);  //get all the data bytes from F-RAM

  digitalWrite(CS,HIGH);  //release chip, signal end of transfer

}

//--------------------------------------------------------------------------------------
// FRAM_SPI_Status_Reg_Write
// Purpose   : API to write to SPI F-RAM
// Arguments : FRAM_data    -> 1 byte status data
// Return    : None
// Usage     : Eg: FRAM_SPI_Status_Reg_Write(0x00); Writes data byte 0x00 to F-RAM status register
//--------------------------------------------------------------------------------------
void FRAM_SPI_Status_Reg_Write(uint8_t FRAM_data)
{
  // F-RAM WRITE ENABLE COMMAND
  digitalWrite(CS,LOW);  //chip select
  SPI.transfer(WREN);    //transmit write enable opcode
  digitalWrite(CS,HIGH); //release chip, signal end transfer

  // F-RAM STATUS WRITE OPERATION

  digitalWrite(CS,LOW);                //chip select
  SPI.transfer(WRSR);                  //transmit status write opcode
  SPI.transfer((uint8_t)(FRAM_data));  //send status byte
  digitalWrite(CS,HIGH);               //release chip, signal end of transfer
}

//--------------------------------------------------------------------------------------
// FRAM_SPI_Status_Reg_Read
// Purpose   : API to read status byte from SPI F-RAM
// Arguments : None
// Return    : 1 byte status data read from F-RAM
// Usage     : Eg: status = FRAM_SPI_Status_Reg_Read(); Reads 1-byte status data from F-RAM and assigns to
//           : variable 'status'
//--------------------------------------------------------------------------------------
uint8_t FRAM_SPI_Status_Reg_Read(void)
{
  uint8_t data;

  //FRAM STATUS READ OPERATION

  digitalWrite(CS,LOW);      //chip select
  SPI.transfer(RDSR);        //transmit read opcode
  data = SPI.transfer(0xFF); //get status data byte from F-RAM
  digitalWrite(CS,HIGH);     //release chip, signal end of transfer

  return data;
}

