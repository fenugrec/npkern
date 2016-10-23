#ifndef _PLATF_H
#define _PLATF_H
/****** Platform-specific code and defines 
 */

/* (c) copyright fenugrec 2016
 * GPLv3
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/****** kernel customization ******/
#define SCI_DEFAULTDIV 9	//default value for BRR reg  == (20 * 1000 / (32 * 62.5)) -1

/* Uncomment to enable verification of succesful block erase . Adds 128B for the block descriptors + ~ 44B of code */
//#define POSTERASE_VERIFY



#include <stdbool.h>
#include "reg_defines/7055_7058_180nm.h"	//io peripheral regs etc

/* where the pre-ramjump metadata is stored (wdt pin, s36k2, etc) */
#define RAMJUMP_PRELOAD_META 0xffff8000

/*** WDT and master clock stuff */
#define WDT_PER_MS	2
	/* somehow shc sucks at reducing the following :
	 * u16 WDT_MAXCNT = WDT_PER_MS / 1000 * (40*1000*1000ULL / 64))
	 */
//#define WDT_MAXCNT WDT_PER_MS * 40*1000*1000UL / 64 / 1000
#define WDT_MAXCNT 1250

#define MCLK_GETMS(x) ((x) * 16 / 10000)	/* convert ticks to milliseconds */
#define MCLK_GETTS(x) ((x) * 10000 / 16) /* convert millisec to ticks */
#define MCLK_MAXSPAN	10000	/* arbitrary limit (in ms) for time spans measured by MCLK */
/** Get current timestamp from free-running counter */
//uint32_t get_mclk_ts(void);
#define get_mclk_ts(x) (ATU0.TCNT)


/*********  Reflashing defines
 *
 * These are for SH7058 and SH7055 (0.18um), and assume this RAM map :
 *
 * - stack @ 0xFFFF BFFC (growing downwards)
 * - kernel @ 0xFFFF 8100, this leaves ~16k for both kernel + stack
 * and according to mcu type:
 * - mcu's built-in erase and write programs copied @ 0xFFFF1000 or 0xFFFF7000
 */


/* Select area in which to download the erase + write microcode.
 * skip 00 and 01 in case someone wants to use RAMER at some point
 */

#define FTDAR_ERASE 0x02
#define FTDAR_WRITE 0x03

#if defined(SH7058)
#define FL_ERASE_BASE	0xFFFF1000
#define FL_WRITE_BASE	0xFFFF1800

#define FL_MAXROM	(1024*1024UL - 1UL)

#define RAM_MIN	0xFFFF0000
#define RAM_MAX 	0xFFFFBFFF

#elif defined(SH7055_18)

#define FL_ERASE_BASE	0xFFFF7000
#define FL_WRITE_BASE	0xFFFF7800

#define FL_MAXROM	(512*1024UL - 1UL)

#define RAM_MIN	0xFFFF6000
#define RAM_MAX	0xFFFFDFFF

#else
#error No target specified !

#endif


#define FL_FPEFEQ	(40 * 100)
#define FL_ERASEBLOCKS	15	//EB0...EB15; see DS



/**** ROM erase block defs ****/
struct flashblock {
	uint32_t start;
	uint32_t len;
};


/* these assume that gcc respects this part of the renesas calling convention :
 * r0 : function return value
 * r4 : first arg
 * r5 : second arg
 */

static uint32_t (*const fl_erase_init)(uint32_t FPEFEQ, uint32_t FUBRA) = (void *) FL_ERASE_BASE + 32;
static uint32_t (*const fl_write_init)(uint32_t FPEFEQ, uint32_t FUBRA) = (void *) FL_WRITE_BASE + 32;
static uint32_t (*const fl_erase)(uint32_t FEBS) = (void *) FL_ERASE_BASE + 16;
/** FMPAR : destination in ROM;
 * FMPDR : source data
 */
static uint32_t (*const fl_write)(uint32_t FMPDR, uint32_t FMPAR) = (void *) FL_WRITE_BASE + 16;

/** Ret 1 if ok
 *
 * sets *err to a negative response code if failed
 */
bool platf_flash_init(u8 *err);


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
 */
uint32_t platf_flash_wb(uint32_t dest, uint32_t src, uint32_t len);

/***** Init funcs ****/


/** init platform-specific stuff : SCI, clocks, interrupts */
void init_platf(void);

/** init necessary timers & shit for WDT interrupt !
 */
void init_wdt(void);


/** spinloop for X millisecs */
//void waitms(unsigned ms);

#endif

