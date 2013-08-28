//-----------------------------------------------------------------------------
// Fxxx_Flash_Interface.c
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// File Description:
//
// This file is part of the EEPROM emulation example code, and defines the 
// flash write/erase interface.
//
// Release 1.0 / 10NOV2010 (BD)
//    -Initial Revision
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Fxxx_EEPROM_Configuration.h"

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void FLASH_WriteErase (U16, S8, S8);
U8 FLASH_BlankCheck (U16);
U8 FLASH_Read (U16);

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
SEGMENT_VARIABLE(flashKey1, U8, SEG_DATA) = 0x00;
SEGMENT_VARIABLE(flashKey2, U8, SEG_DATA) = 0x00;
SEGMENT_VARIABLE(flashAddress, U16, SEG_DATA) = FLASH_SAFE_ADDR;

//-----------------------------------------------------------------------------
// FLASH_WriteErase
//-----------------------------------------------------------------------------
//
// This routine writes a byte or erases a page of Flash.
//
// Arguments : U16 address - 16-bit address in code space to write/erase
//             S8 byte - data byte to write (value is don't care on erase)
//             S8 write_erase - 0x01 for writes, 0x03 to erase page
// 
void FLASH_WriteErase (U16 address, S8 byte, S8 write_erase)
{
   bit EA_SAVE = EA;                   // preserve EA
   SEGMENT_VARIABLE_SEGMENT_POINTER(pwrite, S8, SEG_XDATA, SEG_DATA);
   PSBANK_STORE()
   SFRPAGE_SWITCH()

   ENABLE_VDDMON()

   EA = 0;                             // disable interrupts
   write_erase &= 0x03;                // mask off important bits

   flashAddress = address;             // copy passed address
   flashKey2 = 0xF1;                   // set key2

   // check upper address limit
   if (flashAddress > EE_TOP_ADDR)
   {
     flashKey1 = 0x00;                 // clear key1
     flashKey2 = 0x00;                 // clear key2
     flashAddress = FLASH_SAFE_ADDR;   // reset to safe address
     FL_PROTECT()
   }
   // check lower address limit
   else if (flashAddress >= EE_BASE_ADDR)
   {
       flashKey1 = 0xA5;               // set key1

       ENABLE_VDDMON()

       pwrite = (S8 SEG_XDATA *) flashAddress;

       PSBANK_SWITCH()
       ENABLE_FL_MOD()
       PSCTL |= write_erase;           // set up PSEE, PSWE
       *pwrite = byte;                 // write the byte
       PSCTL &= ~0x03;                 // clear PSEE and PSWE
       DISABLE_FL_MOD()
       PSBANK_RESTORE()
       
       flashKey2 = 0x00;               // clear key2
       flashKey1 = 0x00;               // clear key1
       flashAddress = FLASH_SAFE_ADDR; // reset to safe address
   }
   else
   {
     FL_PROTECT()
     flashKey1 = 0x00;                 // clear key1
     flashKey2 = 0x00;                 // clear key2
     flashAddress = FLASH_SAFE_ADDR;   // reset to safe address
   }

   EA = EA_SAVE;                       // restore interrupts
   SFRPAGE_RESTORE()
}

//-----------------------------------------------------------------------------
// FLASH_Read
//-----------------------------------------------------------------------------
//
// This routine reads a byte of Flash.
//
// Arguments : U16 address - 16-bit address in code space to read
//
// Returns : U8 Byte that was read
// 
U8 FLASH_Read (U16 address)
{
   U8 dataByte;
   PSBANK_STORE()
   PSBANK_SWITCH()
   dataByte = *((U8 SEG_CODE *) address);   
   PSBANK_RESTORE()
   return dataByte;   
}

//-----------------------------------------------------------------------------
// FLASH_BlankCheck
//-----------------------------------------------------------------------------
//
// This routine checks a page of flash to determine if it is blank.
//
// Arguments :  U16 address - 16-bit base address of Flash page to check
//
// Returns U8 : 0 if not blank, 1 if blank.
//
U8 FLASH_BlankCheck(U16 address)
{
   SEGMENT_VARIABLE_SEGMENT_POINTER(readPointer, U8, SEG_CODE, SEG_DATA);
   U16 pageIndex;
   PSBANK_STORE()
   PSBANK_SWITCH()

   readPointer = (U8 SEG_CODE *)address;
   for (pageIndex = 0; pageIndex < FL_PAGE_SIZE; pageIndex++)
   {
      if (*readPointer != 0xFF)
      {
         PSBANK_RESTORE()
         return 0;
      }
      readPointer++;
   }
   PSBANK_RESTORE()
   return 1;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------