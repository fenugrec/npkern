#ifndef _PLATF_H
#define _PLATF_H
/****** Platform-specific code and defines
 */

/* (c) copyright fenugrec 2016-2020
 * GPLv3
 *
 */


/****** kernel customization ******/
#define SCI_DEFAULTDIV 9	//default value for BRR reg. Speed (kbps) = (20 * 1000) / (32 * (BRR + 1))

/* Uncomment to enable verification of succesful block erase . Adds 128B for the block descriptors + ~ 44B of code */
//#define POSTERASE_VERIFY

/* Uncomment to add diag function for atomic u16 reads */
//#define DIAG_U16READ

/* Uncomment to taint WDT pulse for debug use */
//#define DIAG_TAINTWDT



#include <stdbool.h>

/****** mfg- and mcu-specific defines ******
*
* RAM_MIN, RAM_MAX : whole RAM area
* " #include "reg_defines/????" : i/o peripheral registers
*/

/*** WDT and master clock stuff
 * want to toggle the WDT every X ms (2ms on Nissan)
 */

//#define WDT_PER_MS	2
	/* somehow shc sucks at reducing the following :
	 * u16 WDT_MAXCNT = WDT_PER_MS / 1000 * (40*1000*1000ULL / 64))
	 */
//#define WDT_MAXCNT WDT_PER_MS * 40*1000*1000UL / 64 / 1000



#if defined(npk)
	#define WDT_MAXCNT 1250
/* Nissan only: RAMJUMP_PRELOAD_META : pre-ramjump metadata address */

	#if defined(SH7058)
		#include "reg_defines/7055_7058_180nm.h"
		#define RAM_MIN	0xFFFF0000
		#define RAM_MAX 	0xFFFFBFFF
		#define RAMJUMP_PRELOAD_META 0xffff8000
		#define NPK_SCI SCI1

	#elif defined(SH7055_18)
		#include "reg_defines/7055_7058_180nm.h"
		#define RAM_MIN	0xFFFF6000
		#define RAM_MAX	0xFFFFDFFF
		#define RAMJUMP_PRELOAD_META 0xffff8000
		#define NPK_SCI SCI1

	#elif defined(SH7055_35)
		#include "reg_defines/7055_350nm.h"
		#define RAM_MIN	0xFFFF6000
		#define RAM_MAX	0xFFFFDFFF
		#define RAMJUMP_PRELOAD_META 0xffff8000
		#define NPK_SCI SCI1

	#elif defined(SH7051)
		#include "reg_defines/7051.h"
		#define RAM_MIN	0xFFFFD800
		#define RAM_MAX	0xFFFFFFFF
		#define RAMJUMP_PRELOAD_META 0xffffD800
		#define NPK_SCI SCI2

	#else
		#error No target specified !
	#endif

#elif defined(ssmk)
	#define WDT_MAXCNT 4125 //aim for 6.6ms , although it probably works at 2ms anyway
	#if defined(SH7058)
		#include "reg_defines/7055_7058_180nm.h"
		#define RAM_MIN	0xFFFF0000
		#define RAM_MAX 	0xFFFFBFFF
		#define NPK_SCI SCI2

	#elif defined(SH7055_18)
		#include "reg_defines/7055_7058_180nm.h"
		#define RAM_MIN	0xFFFF6000
		#define RAM_MAX	0xFFFFDFFF
		#define NPK_SCI SCI2
	
	#else
		#error invalid target for ssmk
	#endif
#endif



#define MCLK_GETMS(x) ((x) * 16 / 10000)	/* convert ticks to milliseconds */
#define MCLK_GETTS(x) ((x) * 10000 / 16) /* convert millisec to ticks */
#define MCLK_MAXSPAN	10000	/* arbitrary limit (in ms) for time spans measured by MCLK */
/** Get current timestamp from free-running counter */
//uint32_t get_mclk_ts(void);
#define get_mclk_ts(x) (ATU0.TCNT)


/** Ret 1 if ok
 *
 * sets *err to a negative response code if failed
 */
bool platf_flash_init(uint8_t *err);


/** Enable modification (erase/write) to flash.
 *
 * If this is not called after platf_flash_init(), the actual calls to erase / write flash are skipped
 */
void platf_flash_unprotect(void);

/** Erase block, see definition of blocks in DS.
 *
 * ret 0 if ok
 */
uint32_t platf_flash_eb(unsigned blockno);

/** Write block of data. len must be multiple of SIDFL_WB_DLEN
 *
 *
 * @return 0 if ok , response code ( > 0x80) if failed.
 *
 * Note : the implementation must not assume that the src address will be 4-byte aligned !
 */
uint32_t platf_flash_wb(uint32_t dest, uint32_t src, uint32_t len);

/***** Init funcs ****/


/** init platform-specific stuff : SCI, clocks, interrupts, WDT etc */
void init_platf(void);



/** force reset by external supervisor and/or internal WDT.
 */
void die(void);


#endif

