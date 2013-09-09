/**
 * @file eeprom.c
 * @brief EEPROM emulation interface for Silicon Labs Flash-based 8051 MCUs.
 *
 * @date 27 Aug 2013
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

#include <compiler_defs.h>
#include "flash.h"
#include "eeprom_config.h"


enum {
	PAGE_STATUS_ERASED = 0xFF,
	PAGE_STATUS_RECEIVING = 0xAA,
	PAGE_STATUS_ACTIVE = 0x00
};


/**
 * @struct page_info
 * @brief This structure define an active page infomration
 * @var page_info::idx
 * Member 'idx' is page index within allocated pages.
 * @var page_info::addr
 * Member 'addr' is current page start address
 * @var page_info::tail
 * Member 'tail' is current write pointer offset within this page.
 */
struct page_info{
	U8 idx;
	U16 addr;
	U16 tail;
};

/* EEPROM bitmap operation macro definition*/
#define EE_SET_BITMAP(addr) eeprom_bitmap[(addr) >> 3] |= 1 << ((addr) % 8)
#define EE_CLR_BITMAP(addr) eeprom_bitmap[(addr) >> 3] &= ~(1 << ((addr) % 8))
#define EE_GET_BITMAP(addr) (eeprom_bitmap[(addr) >> 3] & (1 << ((addr) % 8)))

static struct page_info page;


/**
 * @fn static void eeprom_format_page(U16 phy_addr)
 * @brief erase page and write erase count plus 1 in TAG position.
 *	for erase count equal 0xFFFFFF, plus '1' will get 0x1000000, and will only
 *	write 24 bits 0x000000 into flash.
 * @param phy_addr physical page address
 *
 * @return none
 */
static void eeprom_format_page(U16 phy_addr)
{
	UU32 erase_count;
	/* Ignore first byte in page, it is flash status byte*/
	erase_count.U8[0] = 0;
	erase_count.U8[1] = flash_read_byte(phy_addr + 1);
	erase_count.U8[2] = flash_read_byte(phy_addr + 2);
	erase_count.U8[3] = flash_read_byte(phy_addr + 3);
	erase_count.U32 += 1;

	flash_erase_page(phy_addr);

	flash_write_byte(phy_addr + 1, erase_count.U8[1]);
	flash_write_byte(phy_addr + 2, erase_count.U8[2]);
	flash_write_byte(phy_addr + 3, erase_count.U8[3]);
}

/**
 * @fn static U8 eeprom_is_formatted(U16 phy_addr)
 * @brief Check page formatted or not.
 * 
 * @param phy_addr page physical address
 * @return TRUE: page is formatted; FALSE: page is not formatted
 */
static U8 eeprom_is_formatted(U16 phy_addr)
{
    U16 i;
    UU32 tag;
    tag.U8[0] = flash_read_byte(phy_addr++);
    tag.U8[1] = flash_read_byte(phy_addr++);
    tag.U8[2] = flash_read_byte(phy_addr++);
    tag.U8[3] = flash_read_byte(phy_addr++);

    /* Change status is erased or erase count not equal 0xFFFFFF*/
    if((tag.U8[0] != PAGE_STATUS_ERASED) ||
      ((tag.U8[1] == 0xFF)&&(tag.U8[2] = 0xFF)&&(tag.U8[3] = 0xFF))) {
    	return FALSE;
    }

    for (i = EE_TAG_SIZE; i < FL_PAGE_SIZE; i++) {
        if (flash_read_byte(phy_addr++) != 0xFF) {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * @fn static void eeprom_update_page_info(U8 idx, U16 phy_addr, U16 tail)
 * @brief update page structure
 *
 * @param idx current active page index number
 * @param phy_addr current active page physical address
 * @param tail current active page write pointer position
 *
 * @return none
 */
static void eeprom_update_page_info(U8 idx, U16 phy_addr, U16 tail)
{
	page.idx = idx;
	page.addr = phy_addr;
	page.tail = tail;
}

/**
 * @fn static void eeprom_scan_page(U16 phy_addr, U8 idx)
 * @brief scan page and update page information
 *
 * @param phy_addr page physical address,
 * @param idx page index
 */
static void eeprom_scan_page(U16 phy_addr, U8 idx)
{
	U16 tail;
	for (tail = EE_TAG_SIZE; tail < FL_PAGE_SIZE; tail += EE_VARIABLE_SIZE) {
		if( 0xFF == flash_read_byte(phy_addr + tail))
			break;
	}
	eeprom_update_page_info(idx, phy_addr, tail);
}

/**
 * @fn static void eeprom_check_pages(void)
 * @brief Check page status, handle different page status.
 *
 *  The first byte of flash page is status byte. It contains three status:
 *  RECEIVING, ACTIVE, ERASED. We will format RECEIVING page; if ERASED page
 *  is not blank, we will format it too. If we have two more ACTIVE pages,
 *  we will erase full one
 *
 * @return none
 */
static void eeprom_check_pages(void)
{
    U8 i, status, idx, active_pages = 0;
    U16 phy_addr ,active_page_addr = EE_BASE_ADDR;
    for (i = 0; i < FL_PAGES; i++) {
        phy_addr = EE_BASE_ADDR + i * FL_PAGE_SIZE;
        status = flash_read_byte(phy_addr);
        switch (status) {
            case PAGE_STATUS_RECEIVING:
            	eeprom_format_page(phy_addr);
                break;
            case PAGE_STATUS_ERASED:
                if (!eeprom_is_formatted(phy_addr))
                	eeprom_format_page(phy_addr);
                break;
            case PAGE_STATUS_ACTIVE:
                if (active_pages++) {
                    U16 tmp = phy_addr + FL_PAGE_SIZE - EE_VARIABLE_SIZE;
                    /* erase a full contents page*/
                    if (flash_read_byte(tmp) == 0xFF) {
                    	eeprom_format_page(phy_addr);
                    }else{
                    	eeprom_format_page(active_page_addr);
                    	active_page_addr = phy_addr;
                    	idx = i;
                    }
                }else{
                	active_page_addr = phy_addr;
                	idx = i;
                }
                break;
            default:
                break;
        }
    }
    /* If there is no active page, we update page status position with active status flag*/
	if (0 == active_pages)
		flash_write_byte(active_page_addr,PAGE_STATUS_ACTIVE);
	eeprom_scan_page(active_page_addr,idx);
}

/**
 * @fn static U16 eeprom_get_next_page(U8 page_idx)
 * @brief get next available page
 * 
 * @param page_idx page index number
 *
 * @return Next available page address.
 */
static U16 eeprom_get_next_page(U8 page_idx)
{
	U16 dest;
	U8 idx = (page_idx + 1) % FL_PAGES;
	dest =  EE_BASE_ADDR + idx * FL_PAGE_SIZE;
	return dest;
}

/**
 * @fn void flash_copy_page()
 * @brief move valid data from one page to another page.
 *
 * When an active page is full, it will find next available page, and write data
 * in it, and then call this function. It copies data from source page to
 * destination page.
 *
 * @note When calling this function, be aware that destination page already write
 * a pair of the data. Before copy loop start, we need to read it out and set
 * bitmap correspond bit to '1'.
 *
 * @return none
 */
static void flash_copy_page()
{
	U16 src, dest, tail;
	U8 log_addr,idx;
	U8 eeprom_bitmap[EE_BITMAP_SIZE];

	for (idx = 0; idx < EE_BITMAP_SIZE; idx++) {
        eeprom_bitmap[idx] = 0;
    }
	/* Source page scan start from bottom*/
	src = page.addr + FL_PAGE_SIZE - EE_VARIABLE_SIZE;

	dest = eeprom_get_next_page(page.idx);
	/* Mark destination page as receiving status */
	flash_write_byte(dest,PAGE_STATUS_RECEIVING);
	EE_SET_BITMAP(flash_read_byte(dest + EE_TAG_SIZE));
	tail =EE_TAG_SIZE + EE_VARIABLE_SIZE;
	/* Read data from source page and copy it to destination page*/
	while (src >= (page.addr + EE_TAG_SIZE)) {
		log_addr = flash_read_byte(src);
		if (log_addr < EE_SIZE) {
			if (!EE_GET_BITMAP(log_addr)) {
                flash_write_byte(dest + tail, log_addr);
                flash_write_byte(dest + tail + 1, flash_read_byte(src + 1));
				tail += EE_VARIABLE_SIZE;
				EE_SET_BITMAP(log_addr);
			}
		}
		src -= EE_VARIABLE_SIZE;
	}
    /* Mark destination page as active status*/
	flash_write_byte(dest,PAGE_STATUS_ACTIVE);
	/* Erase source page and update erase count in page TAG position*/
	eeprom_format_page(page.addr);
	/* Update page information*/
	idx = (dest - EE_BASE_ADDR) / FL_PAGE_SIZE;
	eeprom_update_page_info(idx, dest, tail);
}

U8 eeprom_init()
{
    eeprom_check_pages();
    return SUCCESS;
}

U8 eeprom_read_byte(U8 log_addr, U8 *byte)
{
	U16 phy_addr;
	if (log_addr >= EE_SIZE)
		return ERROR;

	phy_addr = page.addr + page.tail - EE_VARIABLE_SIZE;
	while(phy_addr >= (page.addr + EE_TAG_SIZE)){
		if (log_addr == flash_read_byte(phy_addr)) {
			*byte = flash_read_byte(phy_addr + 1);
			return SUCCESS;
		}
		phy_addr -= EE_VARIABLE_SIZE;
	}
	*byte = 0xFF;
	return SUCCESS;
}

U8 eeprom_write_byte(U8 log_addr, U8 byte)
{
	U16 phy_addr;
	if (log_addr >= EE_SIZE)
		return ERROR;

	/* The page is full, we need to find a new page*/
	if(page.tail >= FL_PAGE_SIZE) {
		phy_addr = eeprom_get_next_page(page.idx) + EE_TAG_SIZE;
		flash_write_byte(phy_addr, log_addr);
		flash_write_byte(phy_addr + 1, byte);
		flash_copy_page();
	}else{
		phy_addr = page.addr + page.tail;
		flash_write_byte(phy_addr, log_addr);
		flash_write_byte(phy_addr + 1, byte);
		page.tail += EE_VARIABLE_SIZE;
	}
	return SUCCESS;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
