//-----------------------------------------------------------------------------
// Fxxx_Flash_Parameters.h
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// File Description:
//
// This file is used to define the Flash parameters for the selected device,
// include the appropriate device header files, and provide macros for enabling
// the VDD monitor, disabling the watchdog timer, switching SFRPAGE and PSBANK
// where applicable, and enabling /disabling flash writes and erases.
//
// These macros are used in the Fxxx_Flash_Interface.c file to enable building
// the code on different C8051Fxxx platforms.  Devices with 2kB or less of
// Flash are defined in the file but not supported, and will throw an error.
//
// The device being used should be defined where indicated in the "External
// Constants" section of this file.
//
// This file defines parameters for the following device families :
//
// C8051F00x/01x
// C8051F02x
// C8051F04x
// C8051F06x
// C8051F12x/13x
// C8051F2xx
// C8051F30x
// C8051F31x
// C8051F320/1
// C8051F326/7
// C8051F33x
// C8051F34x
// C8051F35x
// C8051F36x
// C8051F41x
// C8051F50x/51x
// C8051F52xA/53xA
// C8051F54x
// C8051F55x/56x/57x
// C8051F58x/59x
// C8051F70x/71x
// C8051F80x/81x/82x/83x
// C8051F90x/91x
// C8051F92x/93x
// C8051F98x/99x
//
// Release 1.0 / 15NOV2010 (BD)
//    -Initial Revision
//

#ifndef Fxxx_FLASH_PARAMETERS_H
#define Fxxx_FLASH_PARAMETERS_H

#include <compiler_defs.h>


//-----------------------------------------------------------------------------
// External Constants
//-----------------------------------------------------------------------------

// Define the device being used (without suffix) below (i.e. C8051F330).  
// A list of supported devices is found at the top of this file.
#define C8051F930


//*** C8051F00x/01x ***
// 32k Versions of C8051F00x/01x
#if (defined C8051F000)||(defined C8051F001) \
  ||(defined C8051F002)||(defined C8051F005) \
  ||(defined C8051F006)||(defined C8051F007) \
  ||(defined C8051F010)||(defined C8051F011) \
  ||(defined C8051F012)||(defined C8051F015) \
  ||(defined C8051F016)||(defined C8051F017)

   #include <c8051f000_defs.h> 
   #define ENABLE_VDDMON()
   #define DISABLE_WDT()      WDTCN = 0xDE; WDTCN = 0xAD;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x7C00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()\  
   if (flashKey1 == 0xA5)\
   {\
      if (flashKey2 != 0xF1)\
      {\
         RSTSRC = 0x22;\
      }\
      else\
      {\
         FLSCL |= 0x01;\
      }\
   }\
   else\
   {\
      RSTSRC = 0x22;\
   }
   #define DISABLE_FL_MOD()   FLSCL &= ~0x01;
   #define FL_PROTECT()       RSTSRC = 0x22;
#endif

// 16k Versions of C8051F00x/01x
#if (defined C8051F018)||(defined C8051F019)

   #include <c8051f000_defs.h> 
   #define ENABLE_VDDMON()
   #define DISABLE_WDT()      WDTCN = 0xDE; WDTCN = 0xAD;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x3C00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()\  
   if (flashKey1 == 0xA5)\
   {\
      if (flashKey2 != 0xF1)\
      {\
         RSTSRC = 0x22;\
      }\
      else\
      {\
         FLSCL |= 0x01;\
      }\
   }\
   else\
   {\
      RSTSRC = 0x22;\
   }
   #define DISABLE_FL_MOD()   FLSCL &= ~0x01;
   #define FL_PROTECT()       RSTSRC = 0x22;
#endif

//*** C8051F02x ***
// 64k Versions of C8051F02x
#if (defined C8051F020)||(defined C8051F021) \
  ||(defined C8051F022)||(defined C8051F023) 

   #include <c8051f020_defs.h> 
   #define ENABLE_VDDMON()
   #define DISABLE_WDT()      WDTCN = 0xDE; WDTCN = 0xAD;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0xFC00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()\  
   if (flashKey1 == 0xA5)\
   {\
      if (flashKey2 != 0xF1)\
      {\
         RSTSRC = 0x22;\
      }\
      else\
      {\
         FLSCL |= 0x01;\
      }\
   }\
   else\
   {\
      RSTSRC = 0x22;\
   }
   #define DISABLE_FL_MOD()   FLSCL &= ~0x01;
   #define FL_PROTECT()       RSTSRC = 0x22;
#endif

//*** C8051F04x ***
// 64k Versions of C8051F04x
#if (defined C8051F040)||(defined C8051F041) \
  ||(defined C8051F042)||(defined C8051F043) \
  ||(defined C8051F044)||(defined C8051F045)

   #include <c8051f040_defs.h> 
   #define ENABLE_VDDMON()    RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      WDTCN = 0xDE; WDTCN = 0xAD;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0xFC00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()\  
   if (flashKey1 == 0xA5)\
   {\
      if (flashKey2 != 0xF1)\
      {\
         RSTSRC = 0x22;\
      }\
      else\
      {\
         FLSCL |= 0x01;\
      }\
   }\
   else\
   {\
      RSTSRC = 0x22;\
   }
   #define DISABLE_FL_MOD()   FLSCL &= ~0x01;
   #define FL_PROTECT()       RSTSRC = 0x22;
#endif

// 32k Versions of C8051F04x
#if (defined C8051F046)||(defined C8051F047)

   #include <c8051f040_defs.h> 
   #define ENABLE_VDDMON()    RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      WDTCN = 0xDE; WDTCN = 0xAD;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x7E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()\  
   if (flashKey1 == 0xA5)\
   {\
      if (flashKey2 != 0xF1)\
      {\
         RSTSRC = 0x22;\
      }\
      else\
      {\
         FLSCL |= 0x01;\
      }\
   }\
   else\
   {\
      RSTSRC = 0x22;\
   }
   #define DISABLE_FL_MOD()   FLSCL &= ~0x01;
   #define FL_PROTECT()       RSTSRC = 0x22;
#endif

//*** C8051F06x ***
// 64k Versions of C8051F06x
#if (defined C8051F060)||(defined C8051F061) \
  ||(defined C8051F062)||(defined C8051F063) \
  ||(defined C8051F064)||(defined C8051F065)

   #include <c8051f060_defs.h> 
   #define ENABLE_VDDMON()    RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      WDTCN = 0xDE; WDTCN = 0xAD;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0xFA00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()\  
   if (flashKey1 == 0xA5)\
   {\
      if (flashKey2 != 0xF1)\
      {\
         RSTSRC = 0x22;\
      }\
      else\
      {\
         FLSCL |= 0x01;\
      }\
   }\
   else\
   {\
      RSTSRC = 0x22;\
   }
   #define DISABLE_FL_MOD()   FLSCL &= ~0x01;
   #define FL_PROTECT()       RSTSRC = 0x22;
#endif

// 32k Versions of C8051F06x
#if (defined C8051F066)||(defined C8051F067)

   #include <c8051f060_defs.h> 
   #define ENABLE_VDDMON()    RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      WDTCN = 0xDE; WDTCN = 0xAD;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x7E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()\  
   if (flashKey1 == 0xA5)\
   {\
      if (flashKey2 != 0xF1)\
      {\
         RSTSRC = 0x22;\
      }\
      else\
      {\
         FLSCL |= 0x01;\
      }\
   }\
   else\
   {\
      RSTSRC = 0x22;\
   }
   #define DISABLE_FL_MOD()   FLSCL &= ~0x01;
   #define FL_PROTECT()       RSTSRC = 0x22;
#endif

//*** C8051F12x/13x ***
// 128k Versions of C8051F12x/13x
#if (defined C8051F120)||(defined C8051F121) \
  ||(defined C8051F122)||(defined C8051F123) \
  ||(defined C8051F124)||(defined C8051F125) \
  ||(defined C8051F126)||(defined C8051F127) \
  ||(defined C8051F130)||(defined C8051F131)

   #include <c8051f120_defs.h> 
   #define ENABLE_VDDMON()    RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      WDTCN = 0xDE; WDTCN = 0xAD;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()     U8 bankSave = PSBANK;
   #define PSBANK_SWITCH()    PSBANK |= 0x30;  // bank 3
   #define PSBANK_RESTORE()   PSBANK = bankSave;
   #define FL_PAGE_SIZE       1024
   #define LOCK_PAGE          0xF800
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()\  
   if (flashKey1 == 0xA5)\
   {\
      if (flashKey2 != 0xF1)\
      {\
         RSTSRC = 0x22;\
      }\
      else\
      {\
         FLSCL |= 0x01;\
      }\
   }\
   else\
   {\
      RSTSRC = 0x22;\
   }
   #define DISABLE_FL_MOD()   FLSCL &= ~0x01;
   #define FL_PROTECT()       RSTSRC = 0x22;
#endif

// 64k Versions of C8051F12x/13x
#if (defined C8051F132)||(defined C8051F133)

   #include <c8051f120_defs.h> 
   #define ENABLE_VDDMON()    RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      WDTCN = 0xDE; WDTCN = 0xAD;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       1024
   #define LOCK_PAGE          0xFC00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()\  
   if (flashKey1 == 0xA5)\
   {\
      if (flashKey2 != 0xF1)\
      {\
         RSTSRC = 0x22;\
      }\
      else\
      {\
         FLSCL |= 0x01;\
      }\
   }\
   else\
   {\
      RSTSRC = 0x22;\
   }
   #define DISABLE_FL_MOD()   FLSCL &= ~0x01;
   #define FL_PROTECT()       RSTSRC = 0x22;
#endif

//*** C8051F2xx ***
// 8k Versions of C8051F2xx
#if (defined C8051F206)||(defined C8051F220) \
  ||(defined C8051F221)||(defined C8051F226) \
  ||(defined C8051F230)||(defined C8051F231) \
  ||(defined C8051F236)

   #include <c8051f200_defs.h> 
   #define ENABLE_VDDMON()
   #define DISABLE_WDT()      WDTCN = 0xDE; WDTCN = 0xAD;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x1C00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()\  
   if (flashKey1 == 0xA5)\
   {\
      if (flashKey2 != 0xF1)\
      {\
         RSTSRC = 0x22;\
      }\
      else\
      {\
         FLSCL |= 0x01;\
      }\
   }\
   else\
   {\
      RSTSRC = 0x22;\
   }
   #define DISABLE_FL_MOD()   FLSCL &= ~0x01;
   #define FL_PROTECT()       RSTSRC = 0x22;
#endif

//*** C8051F30x ***
// 8k Versions of C8051F30x
#if (defined C8051F300)||(defined C8051F301) \
  ||(defined C8051F302)||(defined C8051F303)

   #include <c8051f300_defs.h> 
   #define ENABLE_VDDMON()    RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x1C00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 4k Versions of C8051F30x
#if (defined C8051F304)

   #include <c8051f300_defs.h> 
   #define ENABLE_VDDMON()    RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x1E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 2k Versions of C8051F30x
#if (defined C8051F305)
   #error "Not enough code-eraseable flash to support this function."
#endif

//*** C8051F31x ***
// 16k Versions of C8051F31x
#if (defined C8051F310)||(defined C8051F311) \
  ||(defined C8051F316)||(defined C8051F317)

   #include <c8051f310_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x3C00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 8k Versions of C8051F31x
#if (defined C8051F312)||(defined C8051F313) \
  ||(defined C8051F314)||(defined C8051F315)

   #include <c8051f310_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x1E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F320/1 ***
// 16k Versions of C8051F320/1
#if (defined C8051F320)||(defined C8051F321)

   #include <c8051f320_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x3C00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F326/7 ***
// 16k Versions of C8051F326/7
#if (defined C8051F326)||(defined C8051F327)

   #include <c8051f326_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x3C00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F33x ***
// 16k Versions of C8051F33x
#if (defined C8051F336)||(defined C8051F337) \
  ||(defined C8051F338)||(defined C8051F339)

   #include <c8051f336_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x3C00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 8k Versions of C8051F33x
#if (defined C8051F330)||(defined C8051F331)

   #include <c8051f330_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x1C00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 4k Versions of C8051F33x
#if (defined C8051F332)||(defined C8051F333)

   #include <c8051f330_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x0E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 2k Versions of C8051F33x
#if (defined C8051F334)||(defined C8051F335)
   #error "Not enough code-eraseable flash to support this function."
#endif

//*** C8051F34x ***
// 64k Versions of C8051F34x
#if (defined C8051F340)||(defined C8051F342) \
  ||(defined C8051F344)||(defined C8051F346) \
  ||(defined C8051F34A)||(defined C8051F34C) \
  ||(defined C8051F34D)

   #include <c8051f340_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0xFA00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 32k Versions of C8051F34x
#if (defined C8051F341)||(defined C8051F343) \
  ||(defined C8051F345)||(defined C8051F347) \
  ||(defined C8051F348)||(defined C8051F349) \
  ||(defined C8051F34B)

   #include <c8051f340_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x7E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F35x ***
// 8k Versions of C8051F35x
#if (defined C8051F350)||(defined C8051F351)\
  ||(defined C8051F352)||(defined C8051F353)

   #include <c8051f350_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x1C00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F36x ***
// 32k Versions of C8051F36x
#if (defined C8051F360)||(defined C8051F361) \
  ||(defined C8051F362)||(defined C8051F363) \
  ||(defined C8051F364)||(defined C8051F365) \
  ||(defined C8051F366)||(defined C8051F367)

   #include <c8051f360_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       1024
   #define LOCK_PAGE          0x7800
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 16k Versions of C8051F36x
#if (defined C8051F368)||(defined C8051F369)

   #include <c8051f360_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       1024
   #define LOCK_PAGE          0x3C00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F41x ***
// 32k Versions of C8051F41x
#if (defined C8051F410)||(defined C8051F411)

   #include <c8051f410_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0xA0; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x7C00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 16k Versions of C8051F41x
#if (defined C8051F412)||(defined C8051F413)

   #include <c8051f410_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0xA0; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x3E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F50x/51x ***
// 64k Versions of C8051F50x/51x
#if (defined C8051F500)||(defined C8051F501) \
  ||(defined C8051F502)||(defined C8051F503) \
  ||(defined C8051F508)||(defined C8051F509)

   #include <c8051f500_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0xA0; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0xFA00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 32k Versions of C8051F50x/51x
#if (defined C8051F504)||(defined C8051F505) \
  ||(defined C8051F506)||(defined C8051F507) \
  ||(defined C8051F510)||(defined C8051F511)

   #include <c8051f500_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0xA0; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x7E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F52xA/53xA ***
// 8k Versions of C8051F52xA/53xA
#if (defined C8051F520A)||(defined C8051F521A) \
  ||(defined C8051F530A)||(defined C8051F531A)

   #include <c8051f520_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0xA0; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x1C00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 4k Versions of C8051F52xA/53xA
#if (defined C8051F523A)||(defined C8051F524A) \
  ||(defined C8051F533A)||(defined C8051F534A)

   #include <c8051f520_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0xA0; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x0E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 2k Versions of C8051F52xA/53xA
#if (defined C8051F526A)||(defined C8051F527A) \
  ||(defined C8051F536A)||(defined C8051F537A)
   #error "Not enough code-eraseable flash to support this function."
#endif

//*** C8051F54x ***
// 16k Versions of C8051F54x
#if (defined C8051F540)||(defined C8051F541) \
  ||(defined C8051F542)||(defined C8051F543)

   #include <c8051f540_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0xA0; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x3A00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 8k Versions of C8051F54x
#if (defined C8051F544)||(defined C8051F545) \
  ||(defined C8051F546)||(defined C8051F547)

   #include <c8051f540_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0xA0; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x1E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F55x/56x/57x ***
// 32k Versions of C8051F55x/56x/57x
#if (defined C8051F550)||(defined C8051F551) \
  ||(defined C8051F552)||(defined C8051F553) \
  ||(defined C8051F560)||(defined C8051F561) \
  ||(defined C8051F562)||(defined C8051F563) \
  ||(defined C8051F568)||(defined C8051F569) \
  ||(defined C8051F570)||(defined C8051F571)

   #include <c8051f550_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0xA0; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x7A00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 16k Versions of C8051F55x/56x/57x
#if (defined C8051F554)||(defined C8051F555) \
  ||(defined C8051F556)||(defined C8051F557) \
  ||(defined C8051F564)||(defined C8051F565) \
  ||(defined C8051F566)||(defined C8051F567) \
  ||(defined C8051F572)||(defined C8051F573) \
  ||(defined C8051F574)||(defined C8051F575)

   #include <c8051f550_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0xA0; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x3E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F58x/59x ***
// 128k Versions of C8051F58x/59x
#if (defined C8051F580)||(defined C8051F581) \
  ||(defined C8051F582)||(defined C8051F583) \
  ||(defined C8051F588)||(defined C8051F589)

   #include <c8051f580_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0xA0; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()     U8 bankSave = PSBANK;
   #define PSBANK_SWITCH()    PSBANK |= 0x30;  // bank 3
   #define PSBANK_RESTORE()   PSBANK = bankSave;
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0xFA00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 96k Versions of C8051F58x/59x
#if (defined C8051F584)||(defined C8051F585) \
  ||(defined C8051F586)||(defined C8051F587) \
  ||(defined C8051F590)||(defined C8051F591)

   #include <c8051f580_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0xA0; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()   U8 pageSave = SFRPAGE; SFRPAGE = 0x00;
   #define SFRPAGE_RESTORE()  SFRPAGE = pageSave;
   #define PSBANK_STORE()     U8 bankSave = PSBANK;
   #define PSBANK_SWITCH()    PSBANK &= ~0x30; PSBANK |= 0x20; // bank 2
   #define PSBANK_RESTORE()   PSBANK = bankSave;
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0xFE00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F70x/71x ***
// 16k Versions of C8051F70x/71x
#if (defined C8051F702)||(defined C8051F703) \
  ||(defined C8051F706)||(defined C8051F707) \
  ||(defined C8051F716)||(defined C8051F717)

   #include <c8051f700_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      WDTCN = 0xDE; WDTCN = 0xAD;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x3E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 15k Versions of C8051F70x/71x
#if (defined C8051F700)||(defined C8051F701) \
  ||(defined C8051F704)||(defined C8051F705)

   #include <c8051f700_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      WDTCN = 0xDE; WDTCN = 0xAD;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x3A00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 8k Versions of C8051F70x/71x
#if (defined C8051F708)||(defined C8051F709) \
  ||(defined C8051F710)||(defined C8051F711) \
  ||(defined C8051F712)||(defined C8051F713) \
  ||(defined C8051F714)||(defined C8051F715)

   #include <c8051f700_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      WDTCN = 0xDE; WDTCN = 0xAD;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x1E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F80x/81x/82x/83x ***
// 16k Versions of C8051F80x/81x/82x/83x
#if (defined C8051F800)||(defined C8051F801) \
  ||(defined C8051F802)||(defined C8051F803) \
  ||(defined C8051F804)||(defined C8051F805) \
  ||(defined C8051F806)||(defined C8051F807) \
  ||(defined C8051F808)||(defined C8051F809) \
  ||(defined C8051F810)||(defined C8051F811)

   #include <c8051f800_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x3E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 8k Versions of C8051F80x/81x/82x/83x
#if (defined C8051F812)||(defined C8051F813) \
  ||(defined C8051F814)||(defined C8051F815) \
  ||(defined C8051F816)||(defined C8051F817) \
  ||(defined C8051F818)||(defined C8051F819) \
  ||(defined C8051F820)||(defined C8051F821) \
  ||(defined C8051F822)||(defined C8051F823) \
  ||(defined C8051F824)||(defined C8051F825) \
  ||(defined C8051F826)||(defined C8051F827) \
  ||(defined C8051F828)||(defined C8051F829)

   #include <c8051f800_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x1E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 4k Versions of C8051F80x/81x/82x/83x
#if (defined C8051F830)||(defined C8051F831) \
  ||(defined C8051F832)||(defined C8051F833) \
  ||(defined C8051F834)||(defined C8051F835)

   #include <c8051f800_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x0E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F90x/91x ***
// 16k Versions of C8051F90x/91x
#if (defined C8051F911)||(defined C8051F912)

   #include <c8051f912_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x3A00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 8k Versions of C8051F90x/91x
#if (defined C8051F901)||(defined C8051F902)

   #include <c8051f912_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x1E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F92x/93x ***
// 64k Versions of C8051F92x/93x
#if (defined C8051F930)||(defined C8051F931)

   #include <c8051f930_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       1024
   #define LOCK_PAGE          0xF800
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 32k Versions of C8051F92x/93x
#if (defined C8051F920)||(defined C8051F921)

   #include <c8051f930_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       1024
   #define LOCK_PAGE          0x7C00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

//*** C8051F98x/99x ***
// 8k Versions of C8051F98x/99x
#if (defined C8051F980)||(defined C8051F981) \
  ||(defined C8051F986)||(defined C8051F987) \
  ||(defined C8051F990)||(defined C8051F991) \
  ||(defined C8051F996)||(defined C8051F997)

   #include <c8051f990_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x1E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 4k Versions of C8051F98x/99x
#if (defined C8051F982)||(defined C8051F983) \
  ||(defined C8051F988)||(defined C8051F989)

   #include <c8051f990_defs.h> 
   #define ENABLE_VDDMON()    VDM0CN = 0x80; RSTSRC = (RSTSRC_VAL | 0x02);
   #define DISABLE_WDT()      PCA0MD &= ~0x40;
   #define SFRPAGE_SWITCH()
   #define SFRPAGE_RESTORE()
   #define PSBANK_STORE()
   #define PSBANK_SWITCH()
   #define PSBANK_RESTORE()
   #define FL_PAGE_SIZE       512
   #define LOCK_PAGE          0x0E00
   #define FLASH_SAFE_ADDR    0xFFFF
   #define ENABLE_FL_MOD()    FLKEY  = flashKey1; FLKEY  = flashKey2;
   #define DISABLE_FL_MOD()
   #define FL_PROTECT()       FLKEY = 0x00;
#endif

// 2k Versions of C8051F98x/99x
#if (defined C8051F985)
   #error "Not enough code-eraseable flash to support this function."
#endif

#ifndef FL_PAGE_SIZE
   #error "Specified device definition not found."
#endif

#endif // #ifndef Fxxx_FLASH_PARAMETERS_H

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------