/**
 * @file main.c
 * @brief his file is a simple example usage of the EEPROM Emulation firmware.
 *
 * @date 3 Sep 2013
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
#include "compiler_defs.h"
#include "eeprom_config.h"
#include "eeprom.h"

void main (void) 
{
	U8 tmp;
    SFRPAGE_SWITCH()
    ENABLE_VDDMON()
    DISABLE_WDT()
    SFRPAGE_RESTORE()
    eeprom_init();
    eeprom_read_byte(0, &tmp);
    tmp = 0x55;
    eeprom_write_byte(0,tmp);
    tmp = 0;
    eeprom_read_byte(0, &tmp);
    while(1);
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
