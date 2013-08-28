/* @file Fxxx_EEPROM_Interface.c
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


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <compiler_defs.h>
#include "Fxxx_Flash_Interface.h"
#include "Fxxx_EEPROM_Configuration.h"

//-----------------------------------------------------------------------------
// Internal Constants
//-----------------------------------------------------------------------------

#define NEW_TAG         0xFF        // Value for New Sector
#define WIP_TAG         0x7F        // Value for WIP Sector

#define DONE_TAG_MIN    0x01        // Done tag minimum
#define DONE_TAG_MAX    0x7E        // Done tag maximum

#define NO_TAG          0x00

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
U8 currentEepromSector = 0, currentEepromPage = 0;

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void copySector(U16, U16, U8, U8);
U16 getBaseAddress(U8, U8);
U8 findNextSector(void);
U8 findCurrentSector(void);

U8 EEPROM_WriteBlock (U8, U8 *, U8);
U8 EEPROM_ReadBlock (U8, U8 *, U8);

/*
 * @var eeprom_bitmap[]
 * @breif This is eeprom bitmapping
 *  "1" for updated value, "0" for original value
 */
static U8 eeprom_bitmap[EE_BITMAP_SIZE];
enum {
	PAGE_STATUS_ERASED = 0xFF,
	PAGE_STATUS_RECEIVING = 0xAA,
	PAGE_STATUS_ACTIVE = 0x00
};


/* @struct page_info
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

static struct page_info page;

U8 eeprom_init();
U8 eeprom_write_byte(U8, U8);
U8 eeprom_read_byte(U8, U8 *);
U8 flash_page_copy();

/*
 * @fn U8 eeprom_init()
 * @brief
 *   It restores the pages to good state in case of page status corruption
 * after a power loss or unwanted system reset.And also it will create bit map
 * for those address which has valid value inside. With the bit map, eeprom read
 * function can check the bit map instead of checking contents in eeprom. Which
 * will definitely save time cost.
 *
 * @return 0: success; 1: error
 */
U8 eeprom_init()
{
}

/*
 * @fn U8 eeprom_read_byte(U8 addr, U8 *byte)
 * @brief eeprom byte read interface
 *
 * It read a byte from eeprom
 *
 * @param addr address in eeprom for data read out.
 * @param *byte pointer to byte data read from eeprom.
 *
 * @return 0: success; 1: error
 */
U8 eeprom_read_byte(U8 log_addr, U8 *byte)
{
	if (log_addr >= EE_SIZE)
		return ERROR;
	flash_read_byte();
	return SUCCESS;
}

#define EE_SET_BITMAP(addr) eeprom_bitmap[(addr) >> 3] |= 1 << ((addr) % 8)
#define EE_CLR_BITMAP(addr) eeprom_bitmap[(addr) >> 3] &= ~(1 << ((add) % 8))
#define EE_GET_BITMAP(addr) (eeprom_bitmap[(addr) >> 3] & (1 << ((addr) % 8)))

/*
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
	for (i = EE_TAG_SIZE; i < FL_PAGE_SIZE; i += EE_VRIABLE_SIZE) {
		log_addr = flash_read_byte(phy_addr + i);
		if (log_addr != 0xFF) {
			log_addr %= EE_SIZE;
			EE_SET_BITMAP(log_addr);
		}
	}
}
/*
 * @fn U8 eeprom_write_byte(U8 log_addr, U8 byte)
 * @brief eeprom byte write interface
 *
 * It writes a byte to eeprom
 *
 * @param log_addr address in eeprom for data write in.
 * @param byte byte data write into eeprom.
 *
 * @return 0: success; 1: error
 */
U8 eeprom_write_byte(U8 log_addr, U8 byte)
{
	if (log_addr >= EE_SIZE)
		return ERROR;

	if (page.tail >= FL_PAGE_SIZE)
		flash_copy_page();

	U16 phy_addr = page.tail + page.addr;
	flash_write_byte(phy_addr,byte);
	page.tail += EE_VRIABLE_SIZE;

	EE_SET_BITMAP(addr);
	return SUCCESS;
}

static void eeprom_format_page(U16 phy_addr)
{
	U32 erase_count;
	erase_count = (flash_read_dword(phy_addr) + 1) | 0xFF000000;
	flash_erase_page(phy_addr);
	flash_write_dword(phy_addr, erase_count);
}

static void eeprom_update_page_info(U8 idx, U16 phy_addr, U16 tail)
{
	page.idx = idx;
	page.addr = phy_addr;
	page.tail = tail;
}

static U16 eeprom_get_next_page(U8 page_idx)
{
	U16 dest;
	U8 idx = (page_idx + 1) % FL_PAGES;
	dest =  EE_BASE_ADDR + idx * FL_PAGE_SIZE;
	return dest;
}

/*
 * @fn U8 flash_copy_page()
 * @brief move valid data from one page to another page.
 *
 * This will be called when an active page is full. It copies data from source page
 * to destination page.
 *
 * @return 0: success; 1: error
 */
static U8 flash_copy_page()
{
	U16 src, dest;
	U8 idx;

	/* Source page scan start from bottom*/
	src = page.addr + FL_PAGE_SIZE;

	dest = eeprom_get_next_page(page.idx);

	/* Mark destination page as receiving status */
	flash_write_byte(dest,PAGE_STATUS_RECEIVING);

	flash_write_byte(dest,PAGE_STATUS_ACTIVE);

	/* Erase source page and update erase count in page TAG position*/
	eeprom_format_page(page.addr);
	//eeprom_update_page_info();
}

//-----------------------------------------------------------------------------
// EEPROM_WriteBlock
//-----------------------------------------------------------------------------
//
// This routine writes the specified array into the emulated EEPROM by
// decoding the address and calling flash write functions.
//
// Arguments :  U8 address - 8-bit EEPROM address to start at
//              U8 *dataBuf - pointer to data array
//              U8 length - number of bytes to write
//
// Returns U8 : EE_NO_ERROR = Write was sucessful
//              EE_WRITE_ERROR = Write did not succeed, due to either a page
//                erase problem or an out-of-bounds address.
//              EE_SECTOR_ERROR = Could not determine current sector.
//
U8 EEPROM_WriteBlock (U8 address, U8 *dataBuf, U8 length)
{
   U16 readAddr, writeAddr;
   U8 nextSector;
   U8 currentTag;
   U8 byteCount;
   U8 pageEraseCounter;

   if ((U16)(address+length) > EE_SIZE)
   {
      return EE_WRITE_ERROR;
   }

   currentTag = findCurrentSector();
   if (currentTag == NO_TAG)
   {
      return EE_SECTOR_ERROR;
   }

   // Get the current sector's base address for read (copy) later
   readAddr = getBaseAddress(currentEepromPage, currentEepromSector);

   nextSector = findNextSector();

   // Get base address of sector where we will be writing
   writeAddr = getBaseAddress(currentEepromPage, currentEepromSector);

   if (nextSector == 0x00)
   {
      pageEraseCounter = 0;

      while (pageEraseCounter < FL_ERASE_LIMIT)
      {
         FLASH_WriteErase (writeAddr, 0xFF, FL_ERASE);
         if (!FLASH_BlankCheck(writeAddr))
         {
            if (++pageEraseCounter == FL_ERASE_LIMIT)
            {
               return EE_WRITE_ERROR;
            }
         }
         else
         {
            break;
         }
      }
   }

   // Mark new sector as copy in progress
   FLASH_WriteErase (writeAddr + TAG_OFFSET, WIP_TAG, FL_WRITE);

   // Copy from old to new sector excluding copy addresses
   copySector(readAddr, writeAddr, address, length);

   // Write new info to sector
   for (byteCount = 0; byteCount < length; byteCount++)
   {
      FLASH_WriteErase (writeAddr + address + byteCount, *dataBuf++, FL_WRITE);
   }
   if (currentTag == DONE_TAG_MAX)
   {
      currentTag = DONE_TAG_MIN;
   }
   else
   {
      currentTag++;
   }

   // Mark new sector with valid tag
   FLASH_WriteErase (writeAddr + TAG_OFFSET, currentTag, FL_WRITE);

   return EE_NO_ERROR;
}

//-----------------------------------------------------------------------------
// EEPROM_ReadBlock
//-----------------------------------------------------------------------------
//
// This routine reads the emulated EEPROM information into the specified
// buffer.
//
// Arguments :  U8 address - 8-bit EEPROM address to start at
//              U8 *dataBuf - pointer to data array
//              U8 length - number of bytes to read
//
// Returns U8 : EE_NO_ERROR = Read was sucessful
//              EE_READ_ERROR = Read did not succeed, due to  out-of-bounds
//                 address.
//              EE_SECTOR_ERROR = Could not determine current sector.
//
U8 EEPROM_ReadBlock (U8 address, U8 *dataBuf, U8 length)
{
   U16 sectorAddress;
   U8 byteCount;

   if ((U16)(address+length) > EE_SIZE)
   {
      return EE_READ_ERROR;
   }
   if (findCurrentSector() == 0x00)
   {
      return EE_SECTOR_ERROR;
   }
   sectorAddress = getBaseAddress(currentEepromPage, currentEepromSector);
   // Read data bytes
   for (byteCount = 0; byteCount < length; byteCount++)
   {
      *dataBuf++ = FLASH_Read(sectorAddress+address+byteCount);
   }
   return EE_NO_ERROR;
}


//-----------------------------------------------------------------------------
// copySector
//-----------------------------------------------------------------------------
//
// This internal routine copies the contents of one sector to another, while
// excluding the addresses about to be written.
//
// Arguments :  U16 fromAddr - 16-bit Flash address to copy from
//              U16 toAddr - 16-bit Flash addres to copy to
//              U8 exclude - Starting EEPROM address to exclude for copy.
//              U8 length - Number of bytes to exlude from copy.
//
// Returns :    None
//
void copySector(U16 fromAddr, U16 toAddr, U8 exclude, U8 length)
{
   U8 offset_addr;
   U8 copy_byte;

   for (offset_addr = 0; offset_addr < EE_SIZE; offset_addr++)
   {
      //copy_byte = *((U8 SEG_CODE *) (fromAddr+offset_addr));
      copy_byte = FLASH_Read(fromAddr+offset_addr);
      
      if ((offset_addr < exclude)||(offset_addr >= exclude+length))
      {
         FLASH_WriteErase ((toAddr+offset_addr), copy_byte, FL_WRITE);
      }
   }
}

//-----------------------------------------------------------------------------
// findNextSector
//-----------------------------------------------------------------------------
//
// This internal routine finds the next available sector in the EEPROM area
// and updates the currentEepromPage and currentEepromSector variables.
//
// Arguments :  None
//
// Returns U8 : 0 if next page needs to be erased, 1 if empty sector found
//
U8 findNextSector(void)
{
   U8 pageNow = currentEepromPage, sectorNow = currentEepromSector;
   U8 sectorFound = 2;
   U8 sectTag;
   U16 sectorAddress;

   while (sectorFound == 2)
   {
      currentEepromSector++;
      if (currentEepromSector == EE_SECTORS)
      {
         currentEepromSector = 0;
         currentEepromPage++;
         if (currentEepromPage == FL_PAGES)
         {
            currentEepromPage = 0;
         }
      }
      sectorAddress = getBaseAddress(currentEepromPage, currentEepromSector);
      sectTag = FLASH_Read(sectorAddress+TAG_OFFSET);
      if (sectTag == NEW_TAG)
      {
         sectorFound = 1;
      }
      else if ((pageNow == currentEepromPage)&&
               (sectorNow == currentEepromSector))
      {
         currentEepromSector = 0;
         currentEepromPage++;
         if (currentEepromPage == FL_PAGES)
         {
            currentEepromPage = 0;
         }
         sectorFound = 0;
      }
   }
   return sectorFound;
}

//-----------------------------------------------------------------------------
// findCurrentSector
//-----------------------------------------------------------------------------
//
// This internal routine finds the currently used sector in the EEPROM area
// and updates the currentEepromPage and currentEepromSector variables.
//
// Arguments :  None
//
// Returns U8 : 0 if sector could not be determined, sector tag if sector
//              was found
//
U8 findCurrentSector(void)
{
   U8 pgCount, sectCount;
   U8 emptySectors = 0;
   U8 sectTag = NO_TAG, latestTag = NO_TAG, nextTag = NO_TAG;

   for (pgCount = 0; pgCount < FL_PAGES; pgCount++)
   {
      for (sectCount = 0; sectCount < EE_SECTORS; sectCount++)
      {
         sectTag = FLASH_Read(getBaseAddress(pgCount, sectCount)+TAG_OFFSET);

         if (sectTag == NEW_TAG)
         {
            emptySectors++;
         }
         if ((sectTag >= DONE_TAG_MIN)&&(sectTag <= DONE_TAG_MAX))
         {
            if ((latestTag == NO_TAG)||(sectTag == nextTag))
            {
               latestTag = sectTag;
               currentEepromPage = pgCount;
               currentEepromSector = sectCount;

               if (sectTag == DONE_TAG_MAX)
               {
                  nextTag = DONE_TAG_MIN;
               }
               else
               {
                  nextTag = sectTag + 1;
               }
            }
         }
      }
   }
   // If all sectors are empty, start fresh
   if (emptySectors == FL_PAGES*EE_SECTORS)
   {
      latestTag = DONE_TAG_MIN;
      currentEepromPage = FL_PAGES-1;
      currentEepromSector = EE_SECTORS-1;
   }

   return latestTag;
}

//-----------------------------------------------------------------------------
// getBaseAddress
//-----------------------------------------------------------------------------
//
// This internal routine calculates the base address for a Flash sector in the
// emulated EEPROM area.
//
// Arguments :  U8 page - zero-based page number to calculate from.
//              U8 sector - zero-based sector number to calculate from.
//
// Returns U16: 16-bit Flash address corresponding to the page and sector.
//
U16 getBaseAddress(U8 page, U8 sector)
{
   return (EE_BASE_ADDR+(FL_PAGE_SIZE*page)+(EE_SIZE+EE_TAG_SIZE)*sector);
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
