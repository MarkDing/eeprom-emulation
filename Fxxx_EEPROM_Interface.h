//-----------------------------------------------------------------------------
// Fxxx_EEPROM_Interface.h
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// File Description:
//
// This file is the header to include all external functions and variables for
// the emulated EEPROM interface defined in Fxxx_EEPROM_Interface.c.
//
//
// Release 1.0 / 10NOV2010 (BD)
//    -Initial Revision
//

#ifndef Fxxx_EEPROM_INTERFACE_H
#define Fxxx_EEPROM_INTERFACE_H

//-----------------------------------------------------------------------------
// External Function PROTOTYPES
//-----------------------------------------------------------------------------
extern U8 EEPROM_WriteBlock (U8, U8 *, U8);
extern U8 EEPROM_ReadBlock (U8, U8 *, U8);

#endif // #ifndef Fxxx_EEPROM_INTERFACE_H

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------