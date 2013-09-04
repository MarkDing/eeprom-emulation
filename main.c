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

U8 test_buf[32];
void main (void) 
{
	U16 i;
	U8 tmp;
    SFRPAGE_SWITCH()
    ENABLE_VDDMON()
    DISABLE_WDT()
    SFRPAGE_RESTORE()
    eeprom_init();
    for(i=0;i< EE_SIZE ;i++) {
    	eeprom_read_byte(i,&test_buf[i]);
    }
    for(i=0;i< EE_SIZE - 2;i++) {
    	test_buf[i] = i;
    	eeprom_write_byte(i, test_buf[i]);
    }
    tmp = 0;
    while(tmp < 20){
        for(i=0;i< EE_SIZE - 2;i++) {
        	test_buf[i] = i + 1;
        	eeprom_write_byte(i, test_buf[i]);
        }
        tmp++;
    }
    while(1);
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
