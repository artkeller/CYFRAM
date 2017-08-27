/*
 * Copyright (c) 2014 by Cypress Semiconductor Technology Pvt Ltd. <medu@cypress.com>
 * SPI F-RAM Example Project.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

// Update by artkeller@gmx.de 20170826

#include <SPI.h>
#include <CYSPIFRAM.h>

// Data buffer size
#define BUFFER_SIZE            (16u)

// Example F-RAM Address
#define EXAMPLE_ADDR_1         (0x2000)

// Example F-RAM Address
#define EXAMPLE_ADDR_2         (0x3456)

// Example F-RAM data
#define EXAMPLE_DATA_BYTE      (0xa5)

// Example status register data
#define EXAMPLE_STS_REG_VALUE  (0x08)

// Macro for F-RAM Read/Write PASS/FAIL
#define FAIL                    (0u)
#define PASS                    (1u)

// Initialization
void setup()
{
  uint8_t data_bytes[BUFFER_SIZE];
  uint8_t fram_rd_data[BUFFER_SIZE];
  uint8_t fram_rd_byte;
  uint8_t status_reg = 0;
  uint8_t i; 
  uint8_t result;

  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);

  Serial.print("\nF-RAM SPI Example Project Start");
  Serial.print("\n-------------------------------\n");  
    
  // initialize SPI F-RAM:
  FRAM_SPI_Init();
  
  // Start-up delay
  delay(100);

  // Initialization of variables
  for(i = 0; i < BUFFER_SIZE; i++) 
  {
    fram_rd_data[i] = 0;
    data_bytes[i] = i+1;       
  }
  
  // Write F-RAM data
  Serial.print("\n1. Write Data\t");
  Serial.print(EXAMPLE_DATA_BYTE, HEX);
  
  // F-RAM Write 1 byte of data to F-RAM location EXAMPLE_ADDR_1 
  FRAM_SPI_Write(EXAMPLE_ADDR_1, EXAMPLE_DATA_BYTE);

  // Read F-RAM data
  Serial.print("\n2. Read Data\t");
  
  // F-RAM Read : Read 1 byte of data from F-RAM location EXAMPLE_ADDR_1        
  fram_rd_byte = FRAM_SPI_Read(EXAMPLE_ADDR_1);

  Serial.print(fram_rd_byte, HEX);  

  // Compare and display the result on Serial Monitor
  if(fram_rd_byte == EXAMPLE_DATA_BYTE)
  {
    Serial.print("\nWrite-Read : PASS");
  }
  else
  {
    Serial.print("\nWrite-Read : FAIL");    
  }

  // Write 16 bytes of F-RAM data
  Serial.print("\n3. Write Data Array\n");
  
  // F-RAM Write : Write 16 bytes from array data_bytes to F-RAM location EXAMPLE_ADDR_2
  FRAM_SPI_BurstWrite(EXAMPLE_ADDR_2, data_bytes, BUFFER_SIZE);

  for(i=0;i<BUFFER_SIZE;i++)
  {
    Serial.print(data_bytes[i], HEX);
    Serial.print("  ");    
  }

  // Read and Compare 16 bytes of F-RAM data
  result = PASS;  
  Serial.print("\n4. Read Data Array\n");
  
  FRAM_SPI_BurstRead(EXAMPLE_ADDR_2,fram_rd_data,BUFFER_SIZE);

  for(i=0;i<BUFFER_SIZE;i++)
  {
    if(fram_rd_data[i] != data_bytes[i])
      result = FAIL;
    Serial.print(fram_rd_data[i], HEX);
    Serial.print("  ");
  }

  if(result == PASS)
  {
    Serial.print("\nWrite-Read : PASS");
  }
  else
  {
    Serial.print("\nWrite-Read : FAIL");    
  }
  
  // F-RAM Status Register Write : Write 0x08 to status register
  FRAM_SPI_Status_Reg_Write(EXAMPLE_STS_REG_VALUE);

  Serial.print("\n5. Status Register Write\t");
  Serial.print(EXAMPLE_STS_REG_VALUE, HEX);  
  
  // F-RAM Status Register Read : Read FRAM Status Register
  status_reg = FRAM_SPI_Status_Reg_Read();

  Serial.print("\n6. Status Register Read\t");
  Serial.print(status_reg, HEX);  
  
  if(status_reg == EXAMPLE_STS_REG_VALUE )
  {
      Serial.print("\nStatus Register Write-Read : PASS");
  }
  else
  {
      Serial.print("\nStatus Register Write-Read : PASS");    
  }

  // F-RAM Status Register Write : Write 0x00 to status register
  FRAM_SPI_Status_Reg_Write(0x00);  

  Serial.print("\n\n-----------------------------"); 
  Serial.print("\nF-RAM SPI Example Project End");  

}

// Main
void loop()
{
  // Empty Loop
}

/**
F-RAM SPI Example Project Start
-------------------------------

1. Write Data  A5
2. Read Data  A5
Write-Read : PASS
3. Write Data Array
1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  10  
4. Read Data Array
1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  10  
Write-Read : PASS
5. Status Register Write  8
6. Status Register Read 8
Status Register Write-Read : PASS

-----------------------------
F-RAM SPI Example Project End
 */

