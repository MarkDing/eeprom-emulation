/**
 * @file eeprom_config.h
 * @brief Defines all of the EEPROM parameters for the EEPROM emualtion.
 *
 * @date 2 Sep 2013
 * @version 1.0
 * @author Mark Ding
 *
 ******************************************************************************
 * @section License
 * <b>Copyright (c) 2013 by Silicon Laboratories. http://www.silabs.com</b>
 ******************************************************************************
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Silicon Laboratories End User
 * License Agreement which accompanies this distribution, and is available at
 * http://developer.silabs.com/legal/version/v10/License_Agreement_v10.htm
 * Original content and implementation provided by Silicon Laboratories.
 */
#ifndef __EEPROM_CONFIG_H__
#define __EEPROM_CONFIG_H__

#include "flash_parameters.h"

/**
 * @def FL_PAGES
 * @brief This constant detemines how many pages of Flash to use for the
 *  EEPROM storage area.  The minimum is two.  The emulated EEPROM area
 *  grows up from EE_BASE_ADDRESS and is FL_PAGE_SIZE*FL_PAGES bytes large.
 *  Ensure that the last page is not defined as the lock byte page.
 */
#define FL_PAGES        2

/**
 * @def EE_BASE_ADDR
 * @brief This should point to the memory location where the EEPROM
 *  storage area begins.  This must coincide with the start of a Flash page.
 */
#define EE_BASE_ADDR    (LOCK_PAGE - FL_PAGE_SIZE * FL_PAGES)
/* #define EE_BASE_ADDR    0x1A00*/

/**
 * @def EE_SIZE
 * @brief Defines how many bytes are in the emulated EEPROM.  The maximum
 *  setting is ((FL_PAGE_SIZE - 4) / 4) & 0xF8. It must be 8 bit align.
 */
#define EE_SIZE         16

/**
 * @def EE_BITMAP_SIZE
 * @brief Defines bitmap size which equal EE_SIZE/8
 */
#define EE_BITMAP_SIZE  (EE_SIZE / 8)

/**
 * @def RSTSRC_VAL
 * @brief This should be configured to enable the appropriate reset
 *  sources on the chip.  It will be ORed with 0x02 to enable VDD monitor.
 */
#define RSTSRC_VAL      0x02

#if (EE_SIZE % 8) != 0
#error "Invalid EE_SIZE.  Select an integer multiple of 8."
#endif

#if (EE_BASE_ADDR % FL_PAGE_SIZE) != 0
#error "Invalid EE_BASE_ADDR.  Select an integer multiple of FL_PAGE_SIZE."
#endif

#if (EE_BASE_ADDR + (FL_PAGE_SIZE*FL_PAGES)) > LOCK_PAGE
#error "Defined EE Area not possible.  Reduce EE_BASE_ADDR or FL_PAGES."
#endif


/**
 * The following should not normally need to be edited, unless the guts of the
 * EEPROM routines are being modified.
 */

#define EE_TOP_ADDR     EE_BASE_ADDR + (FL_PAGES*FL_PAGE_SIZE) - 1
#define EE_TAG_SIZE     4    // Number of bytes used for tag info

#define EE_VARIABLE_SIZE    2

#define SUCCESS 0x00
#define ERROR   0x01

#define TRUE 	0x01
#define FALSE	0x00


#endif

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
