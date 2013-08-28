//-----------------------------------------------------------------------------
// Fxxx_ExampleEEPROMUsage.c
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This file is a simple example usage of the EEPROM Emulation firmware.  The
// program reads the current first byte of EEPROM, adds 5 to it, then writes
// the entire EEPROM space with an inverting sequence.  The EEPROM space is
// then read back out into a RAM buffer.
//
//
// Target:         C8051Fxxx Family
// Tool chains:    Keil, Raisonance, SDCC
// Command Line:   None
//
// Release 1.0 / 10NOV2010 (BD)
//    -Initial Revision
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------                 
#include "compiler_defs.h"
#include "Fxxx_EEPROM_Configuration.h"
#include "Fxxx_EEPROM_Interface.h"

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main (void) 
{
	int i;
   SEGMENT_VARIABLE(ramBuffer[EE_SIZE], U8, SEG_DATA);
   U8 bufferCount;
   SFRPAGE_SWITCH()
   ENABLE_VDDMON()
   DISABLE_WDT()
   SFRPAGE_RESTORE()                       

   // Read first byte of EEPROM
   if (EEPROM_ReadBlock(0, ramBuffer, 1) == EE_NO_ERROR)
   {
      while(1);      // Error reading EEPROM
   }
   ramBuffer[0] += 5;

   // Fill RAM buffer with an inverting sequence.
   for (bufferCount = 1; bufferCount < EE_SIZE; bufferCount++)
   {
      ramBuffer[bufferCount] = ~ramBuffer[bufferCount-1];
   }

   // Write RAM buffer to EEPROM.
   if (EEPROM_WriteBlock (0, ramBuffer, EE_SIZE) != EE_NO_ERROR)
   {
      while(1);      // Error writing EEPROM
   }

   for(i = 0; i < EE_SIZE; i++)
   {
   	ramBuffer[i] = 0;
   }
   // Read EEPROM contents back into RAM 
   if (EEPROM_ReadBlock(0, ramBuffer, EE_SIZE) != EE_NO_ERROR)
   {
      while(1);      // Error reading EEPROM
   }

   while(1);         // No errors occured
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------