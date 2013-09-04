/**
 * @file flash.h
 * @brief Flash read/write/erase interface of EEPROM emulation.
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
#ifndef __FLASH_H__
#define __FLASH_H__

/**
 * @fn void flash_erase_page(U16 address)
 * @brief erase a flash page.
 *
 * @param address flash page address to be erased
 *
 */
extern void flash_erase_page(U16 address);

/**
 * @fn void flash_write_byte(U16 address, U8 dat)
 * @brief Write a byte into flash.
 *
 * @param address physical address in flash
 * @param dat data byte to write
 */
extern void flash_write_byte(U16 address, U8 dat);

/**
 * @fn U8 flash_read_byte(U16 address)
 * @brief Read a byte from flash
 *
 * @param address physical address in flash
 *
 * @return dat data byte read from flash
 */
extern U8 flash_read_byte(U16 address);

#endif

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
