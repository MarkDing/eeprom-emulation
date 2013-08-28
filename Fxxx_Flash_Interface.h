//-----------------------------------------------------------------------------
// Fxxx_Flash_Interface.h
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// File Description:
//
// This file is the header to include all external functions and variables
// defined in the Flash_Interface.c files.  The appropriate flash interface
// code file for the device should be included in the project.
//
// Release 1.0 / 10NOV2010 (BD)
//    -Initial Revision
//

#ifndef Fxxx_FLASH_INTERFACE_H
#define Fxxx_FLASH_INTERFACE_H

//-----------------------------------------------------------------------------
// External Function PROTOTYPES
//-----------------------------------------------------------------------------

extern void FLASH_WriteErase (U16, S8, S8);
extern U8 FLASH_BlankCheck(U16);
extern U8 FLASH_Read (U16);

//-----------------------------------------------------------------------------
// External Variables
//-----------------------------------------------------------------------------

extern SEGMENT_VARIABLE(FlashKey1, U8, SEG_DATA);
extern SEGMENT_VARIABLE(FlashKey2, U8, SEG_DATA);

//-----------------------------------------------------------------------------
// External Constants
//-----------------------------------------------------------------------------

#define FL_WRITE        0x01        // PSCTL mask for Flash Writes
#define FL_ERASE        0x03        // PSCTL mask for Flash Erase

#endif // #ifndef Fxxx_FLASH_INTERFACE_H

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------