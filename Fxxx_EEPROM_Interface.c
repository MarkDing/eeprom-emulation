/**
 * @file Fxxx_EEPROM_Interface.c
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
#include "Fxxx_Flash_Interface.h"
#include "Fxxx_EEPROM_Configuration.h"


/**
 * @var eeprom_bitmap[]
 * @brief This is eeprom bitmapping
 *  "1" for updated value, "0" for original value
 */
static U8 eeprom_bitmap[EE_BITMAP_SIZE];
enum {
	PAGE_STATUS_ERASED = 0xFF,
	PAGE_STATUS_RECEIVING = 0xAA,
	PAGE_STATUS_ACTIVE = 0x00
};


/**
 * @struct page_info
 * @breif This structure define an active page infomration
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
#define EE_CLR_BITMAP(addr) eeprom_bitmap[(addr) >> 3] &= ~(1 << ((add) % 8))
#define EE_GET_BITMAP(addr) (eeprom_bitmap[(addr) >> 3] & (1 << ((addr) % 8)))

static struct page_info page;


/**
 * @fn static void eeprom_format_page(U16 phy_addr)
 * @brief erase page and write erase count plus 1 in TAG position
 *
 * @param phy_addr physical page address
 *
 * @return none
 */
static void eeprom_format_page(U16 phy_addr)
{
	UU32 erase_count;
	/* Ignore first byte in page, it is flash status byte*/
	erase_count.U8[1] = flash_read_byte(phy_addr + 1);
	erase_count.U8[2] = flash_read_byte(phy_addr + 2);
	erase_count.U8[3] = flash_read_byte(phy_addr + 3);
	erase_count.U32++;

	flash_erase_page(phy_addr);

	flash_write_byte(phy_addr + 1, erase_count.U8[1]);
	flash_write_byte(phy_addr + 2, erase_count.U8[2]);
	flash_write_byte(phy_addr + 3, erase_count.U8[3]);
}

/**
 * @fn static U8 eeprom_is_blank(U16 phy_addr)
 * @brief check page whether is blank, it ignores the TAG 4 bytes.
 * 
 * @param phy_addr page physical address
 * @return TRUE: page is blank; FALSE: page is not blank
 */
static U8 eeprom_is_blank(U16 phy_addr)
{
    U16 i;
    for (i = EE_TAG_SIZE; i < FL_PAGE_SIZE; i++) {
        if (flash_read_byte(phy_addr + i) != 0xFF) {
            return FALSE;
        }
    }
    return TRUE;
}
/**
 * @fn static void eeprom_check_pages(void)
 * @brief check page status, handle different page status
 *
 * @return none
 */
static void eeprom_check_pages(void)
{
    U8 i, status, active_pages = 0;
    U16 phy_addr ,active_page_addr;
    for (i = 0; i < FL_PAGES; i++) {
        phy_addr = EE_BASE_ADDR + i * FL_PAGE_SIZE;
        status = flash_read_byte(phy_addr);
        switch (status) {
            case PAGE_STATUS_RECEIVING:
            	eeprom_format_page(phy_addr);
                break;
            case PAGE_STATUS_ERASED:
                if (!eeprom_is_blank(phy_addr)) {
                	eeprom_format_page(phy_addr);
                }
                break;
            case PAGE_STATUS_ACTIVE:
                if (active_pages) {
                    U16 tmp = phy_addr + FL_PAGE_SIZE - EE_VARIABLE_SIZE;
                    /* erase a full contents page*/
                    if (flash_read_byte(tmp) == 0xFF) {
                        flash_erase_page(phy_addr);
                    }else{
                        flash_erase_page(active_page_addr);
                    }
                } else {
                    active_pages++;
                    active_page_addr = phy_addr;
                }
                break;
            default:
                break;
        }
    }
}


/**
 * @fn static void eeprom_bitmap_init()
 * @brief init eeprom_bitmap all zero
 * 
 * @return none
 */
static void eeprom_bitmap_init()
{
    U8 i;
    for (i = 0; i < EE_BITMAP_SIZE; i++) {
        eeprom_bitmap[i] = 0;
    }
}
/**
 * @fn static void eeprom_creat_bitmap(U16 phy_addr)
 * @brief create eeprom bitmap by scanning flash page
 *
 * @param phy_addr flash physical address
 *
 * @return none
 */
static void eeprom_creat_bitmap(U16 phy_addr)
{
	U8 log_addr;
	U16 i;
	for (i = EE_TAG_SIZE; i < FL_PAGE_SIZE; i += EE_VARIABLE_SIZE) {
		log_addr = flash_read_byte(phy_addr + i);
		if ((log_addr != 0xFF) && (log_addr < EE_SIZE)) {
			log_addr %= EE_SIZE;
			EE_SET_BITMAP(log_addr);
		}
	}
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
 * This will be called when an active page is full. It copies data from source page
 * to destination page.
 *
 * @return none
 */
static void flash_copy_page()
{
	U16 src, dest, tail;
	U8 log_addr,tmp;

	/* Source page scan start from bottom*/
	src = page.addr + FL_PAGE_SIZE - EE_VARIABLE_SIZE;

	dest = eeprom_get_next_page(page.idx);
	/* Mark destination page as receiving status */
	flash_write_byte(dest,PAGE_STATUS_RECEIVING);
	tail = dest + EE_TAG_SIZE;
	/* Read data from source page and copy it to destination page*/
	while (src > (page.addr + EE_TAG_SIZE)) {
		log_addr = flash_read_byte(src);
		if (log_addr < EE_SIZE) {
			/* Once we copy one data from source page, we clear it in bitmap.
			 * Then we will copy latest copy only for that data*/
			if (EE_GET_BITMAP(log_addr)) {
				tmp = flash_read_byte(src);
                flash_write_byte(tail, tmp);
				tmp = flash_read_byte(src + 1);
                flash_write_byte(tail + 1, tmp);
				tail += EE_VARIABLE_SIZE;
				EE_CLR_BITMAP(log_addr);
			}
		}
		src -= EE_VARIABLE_SIZE;
	}
    /* Mark destination page as active status and create bitmap for it*/
	flash_write_byte(dest,PAGE_STATUS_ACTIVE);
	eeprom_creat_bitmap(dest);

	/* Erase source page and update erase count in page TAG position*/
	eeprom_format_page(page.addr);
	U8 idx = (page.idx + 1) % FL_PAGES;
	eeprom_update_page_info(idx, dest,tail);
}

U8 eeprom_init()
{
    eeprom_update_page_info(0,EE_BASE_ADDR,0);
    eeprom_bitmap_init();
    eeprom_check_pages();
}

U8 eeprom_read_byte(U8 log_addr, U8 *byte)
{
	if (log_addr >= EE_SIZE)
		return ERROR;

	/* Not in bitmap, that mean it keeps original value 0xFF*/
	if(0 == EE_GET_BITMAP(log_addr)) {
		*byte = 0xFF;
		return SUCCESS;
	}

	U16 phy_addr = page.addr + page.tail - EE_VARIABLE_SIZE;
	while(phy_addr > (page.addr + EE_TAG_SIZE)){
		if (log_addr == flash_read_byte(phy_addr)) {
			*byte = flash_read_byte(phy_addr + EE_ADDR_SIZE);
			return SUCCESS;
		}
		phy_addr -= EE_VARIABLE_SIZE;
	}
	return ERROR;
}


U8 eeprom_write_byte(U8 log_addr, U8 byte)
{
	if (log_addr >= EE_SIZE)
		return ERROR;

	if (page.tail >= FL_PAGE_SIZE)
		flash_copy_page();

	U16 phy_addr = page.tail + page.addr;
	flash_write_byte(phy_addr, log_addr);
	flash_write_byte(phy_addr, byte);
	page.tail += EE_VARIABLE_SIZE;
	EE_SET_BITMAP(phy_addr);
	return SUCCESS;
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
