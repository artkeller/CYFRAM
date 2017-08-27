/*
 * Copyright (c) 2014 by Cypress Semiconductor Technology Pvt Ltd. <medu@cypress.com>
 * I2C F-RAM Example Project.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

// Update by artkeller@gmx.de 20170826

#include <Wire.h> 
#include <CYI2CFRAM.h>

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

// F-RAM Slave Address
#define FRAM_SLAVE_SRAM_ADDR    (0x50)

// Macro for F-RAM Read/Write PASS/FAIL
#define FAIL                    (0u)
#define PASS                    (1u)

// I2C Communication Error
#define COM_ERR()  for(;;){}

// Initialization
void setup()
{
    uint8_t data_bytes[BUFFER_SIZE];
    uint8_t fram_rd_data[BUFFER_SIZE];
    uint8_t fram_wr_byte;
    uint8_t fram_rd_byte, fram_rd_current_byte;        
    uint8_t i; 
    uint8_t result;
    uint8_t sts;
    uint32_t no_of_bytes_read;
  
    // Initialize serial communication at 9600 bits per second:
    Serial.begin(115200);
  
    Serial.print("\nF-RAM I2C Example Project Start");
    Serial.print("\n-------------------------------\n");  
    
    // Initialize I2C F-RAM
    FRAM_I2C_Init();
    Wire.setClock(800000);
    
    // Start-up Delay
    delay(100);

    // Initialization of variables
    for(i = 0; i < BUFFER_SIZE; i++)
    {
        data_bytes[i] = i+1;
        fram_rd_data[i] = 0;
    }

    // Write F-RAM data
    Serial.print("\n1. Write Data\t");
    Serial.print(EXAMPLE_DATA_BYTE, HEX);
        
    // F-RAM Write : Write 1 byte of data to F-RAM location EXAMPLE_ADDR_1    
    fram_wr_byte = EXAMPLE_DATA_BYTE;
    sts =   FRAM_I2C_Write(FRAM_SLAVE_SRAM_ADDR, EXAMPLE_ADDR_1, &fram_wr_byte, 1);
    if(sts != FRAM_I2C_MSTR_NO_ERROR)
    {
      // I2C Error
      Serial.print("\nCOM ERROR : ");   
      Serial.print(sts, HEX);         
      COM_ERR();
    } 

    // Read a data byte
    Serial.print("\n2. Read Data\t");
    
    // F-RAM Read : Read 1 byte of data from F-RAM location EXAMPLE_ADDR_1        
    no_of_bytes_read = FRAM_I2C_Random_Read(FRAM_SLAVE_SRAM_ADDR, EXAMPLE_ADDR_1,&fram_rd_byte,1);
//    if( no_of_bytes_read > 0)
//    {
//        Serial.print("\nNo. of bytes Read : ");
//        Serial.print(no_of_bytes_read, HEX);  
//  
//        // Read F-RAM data
//        Serial.print("\nData : ");
        Serial.print(fram_rd_byte, HEX);  
//    }
//    else
//    {
//        // Zero bytes read      
//        Serial.print("\nNo. of bytes Read : ");
//        Serial.print(no_of_bytes_read, HEX);        
//    }
  
    // Compare and display the result on Serial Monitor
    if(fram_rd_byte == fram_wr_byte)
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
    sts = FRAM_I2C_Write(FRAM_SLAVE_SRAM_ADDR, EXAMPLE_ADDR_2, data_bytes, BUFFER_SIZE);  
    
    if(sts != FRAM_I2C_MSTR_NO_ERROR)
    {
        // I2C Error
        Serial.print("\nCOM ERROR : ");   
        Serial.print(sts, HEX);
        COM_ERR();
    }    

    for(i=0;i<BUFFER_SIZE;i++)
    {
        Serial.print(data_bytes[i], HEX);
        Serial.print("  ");    
    }

    // Read and Compare 16 bytes of F-RAM data
    result = PASS;  
    Serial.print("\n4. Read Data Array\n");    
  
    // F-RAM Random Read : Read 15 bytes of data from SRAM starting from address EXAMPLE_ADDR_2. //
    //                     Data will be stored into fram_rd_data array.                          //
    
    no_of_bytes_read = FRAM_I2C_Random_Read(FRAM_SLAVE_SRAM_ADDR, EXAMPLE_ADDR_2,fram_rd_data,BUFFER_SIZE-1);
//    if(no_of_bytes_read > 0)
//    {
//        result = PASS;
        for(i=0; i<no_of_bytes_read; i++)
        {
            if(fram_rd_data[i] != data_bytes[i])
                result = FAIL;
            Serial.print(fram_rd_data[i], HEX);
            Serial.print("  ");
        }

//     }   
//    else
//    {
//        // Zero bytes read
//        Serial.print("\nNo. of bytes Read : ");
//        Serial.print(no_of_bytes_read, HEX);        
//    }

     // F-RAM Read : Read the 16th byte through current location read        
     no_of_bytes_read = FRAM_I2C_Current_Read(FRAM_SLAVE_SRAM_ADDR, &fram_rd_current_byte, 1);     
     if(no_of_bytes_read > 0)
     {
         // Read F-RAM data
         Serial.print(fram_rd_current_byte, HEX);  
         
        if(fram_rd_current_byte != data_bytes[BUFFER_SIZE-1])
            result = FAIL;         
     }  

  // Indicate the busrt write / read result
  if(result == PASS)
  {
      Serial.print("\nWrite-Read : PASS");
  }
  else
  {
      Serial.print("\nWrite-Read : FAIL");    
  }
  
  Serial.print("\n\n-----------------------------");  
  Serial.print("\nF-RAM I2C Example Project End");
}

// Main
void loop()
{
  // Empty since there are no periodic functions
}

/**
F-RAM I2C Example Project Start
-------------------------------

1. Write Data  A5
2. Read Data  A5
Write-Read : PASS
3. Write Data Array
1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  10  
4. Read Data Array
1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  10
Write-Read : PASS

-----------------------------
F-RAM I2C Example Project End
 */

/**
F-RAM I2C Example Project Start
-------------------------------

1. Write Data  A5
COM ERROR : 2
 */

