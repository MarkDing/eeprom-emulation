//-----------------------------------------------------------------------------
// Fxxx_EEPROM_Configuration.h
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// File Description:
//
// This file defines all of the EEPROM parameters for the EEPROM emulation
// example code.
//
// Release 1.0 / 10NOV2010 (BD)
//    -Initial Revision
//

#ifndef Fxxx_EEPROM_CONFIGURATION_H
#define Fxxx_EEPROM_CONFIGURATION_H

#include "Fxxx_Flash_Parameters.h"

//-----------------------------------------------------------------------------
// External Constants
//-----------------------------------------------------------------------------
//
// The following constant parameters can be adjusted in this file :
//
// EE_BASE_ADDR - This should point to the memory location where the EEPROM
//   storage area begins.  This must coincide with the start of a Flash page.
//
// FL_PAGES - This constant detemines how many pages of Flash to use for the
//   EEPROM storage area.  The minimum is two.  The emulated EEPROM area
//   grows up from EE_BASE_ADDRESS and is FL_PAGE_SIZE*FL_PAGES bytes large.
//   Ensure that the last page is not defined as the lock byte page.
//
// EE_SIZE - Defines how many bytes are in the emulated EEPROM.  The maximum
//   setting is 255 for this code.
//
// FL_ERASE_LIMIT - This constant detemines how many times the software will 
//   attempt to erase a flash page (and fail) before throwing an error.  The
//   minimum is 1.
//
// RSTSRC_VAL - This should be configured to enable the appropriate reset
//   sources on the chip.  It will be ORed with 0x02 to enable VDD monitor.

#define EE_BASE_ADDR    0x3800      // Base address of EE in Flash
#define FL_PAGES        2           // Pages to use for EEPROM emulation
#define EE_SIZE         32          // EEPROM size in bytes, must be 8 align
#define EE_BITMAP_SIZE  (EE_SIZE / 8)
#define FL_ERASE_LIMIT  1           // Maximum retries on page erase

#define RSTSRC_VAL      0x02        // Set to enable desired reset sources


#if (EE_BASE_ADDR % FL_PAGE_SIZE) != 0
#error "Invalid EE_BASE_ADDR.  Select an integer multiple of FL_PAGE_SIZE."
#endif

#if (EE_BASE_ADDR + (FL_PAGE_SIZE*FL_PAGES)) > LOCK_PAGE
#error "Defined EE Area not possible.  Reduce EE_BASE_ADDR or FL_PAGES."
#endif

//-----------------------------------------------------------------------------
// Additional Constants
//-----------------------------------------------------------------------------
//
// The following should not normally need to be edited, unless the guts of the
// EEPROM routines are being modified.
//

#define EE_TOP_ADDR     EE_BASE_ADDR + (FL_PAGES*FL_PAGE_SIZE) - 1
#define EE_TAG_SIZE     4           // Number of bytes used for tag info
#define TAG_OFFSET      EE_SIZE     // Sector tag offset from base

#define EE_SECTORS      (U8)(FL_PAGE_SIZE/(EE_SIZE+EE_TAG_SIZE))

#define EE_DATA_SIZE        1 // 8 bit data
#define EE_ADDR_SIZE        1 // 8 bit addr
#define EE_VARIABLE_SIZE    (EE_DATA_SIZE + EE_ADDR_SIZE)

#define SUCCESS 0x00
#define ERROR   0x01

#define EE_NO_ERROR     0x00
#define EE_WRITE_ERROR  0x01
#define EE_SECTOR_ERROR 0x02
#define EE_READ_ERROR   0x03

#endif // #ifndef Fxxx_EEPROM_CONFIGURATION_H

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
