/* platform-specific code that implements the reflashing back-end commands
 * (see platf.h)
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


#include "functions.h"
#include "reg_defines/7055_350nm.h"	//io peripheral regs etc

#include <string.h>	//memcmp
#include "stypes.h"
#include "platf.h"
#include "iso_cmds.h"

/*********  Reflashing defines
 *
 * This is for SH7055 (0.35um) and assumes this RAM map (see .ld file)
 *
 * - stack @ 0xFFFF BFFC (growing downwards)
 * - kernel @ 0xFFFF 8100, this leaves ~16k for both kernel + stack
 */


#ifndef SH7055_35
#error Wrong target specified !
#endif

#define FL_MAXROM	(512*1024UL - 1UL)



/********** Timing defs
*/

//Assume 40MHz clock. Some critical timing depends on this being true,
//WDT stuff in particular isn't macro-fied
#define CPUFREQ	(40)

#define WDT_RSTCSR_SETTING 0x5A5F;	//power-on reset if TCNT overflows
#define WDT_TCSR_START (0xA578 | 0x06)	//write value to start with 1:4096 div (26.2 ms @ 40MHz)
#define WDT_TCSR_STOP 0xA558	//write value to stop WDT count

//TODO recheck calculation
#define WAITN_TCYCLE 4		/* clock cycles per loop, see asm */
#define WAITN_CALCN(usec) (((usec) * CPUFREQ / WAITN_TCYCLE) + 1)


/** Common timing constants */
#define TSSWE	WAITN_CALCN(1)
#define TCSWE	WAITN_CALCN(100)

/** Erase timing constants */
#define TSESU	WAITN_CALCN(100)
#define TSE	WAITN_CALCN(10000UL)
#define TCE	WAITN_CALCN(10)
#define TCESU	WAITN_CALCN(10)
#define TSEV	WAITN_CALCN(6)	/******** Renesas has 20 for this !?? */
#define TSEVR	WAITN_CALCN(2)
#define TCEV	WAITN_CALCN(4)


/** Write timing constants */
#define TSPSU	WAITN_CALCN(50)
#define TSP10	WAITN_CALCN(10)
#define TSP30	WAITN_CALCN(10)
#define TSP200	WAITN_CALCN(10)
#define TCP	WAITN_CALCN(5)
#define TCPSU	WAITN_CALCN(5)
#define TSPV	WAITN_CALCN(4)
#define TSPVR	WAITN_CALCN(2)
#define TCPV	WAITN_CALCN(2)


/** FLASH constants */
#define MAX_ET	100		// The number of times of the maximum erase
#define MAX_WT	1000		// The number of times of the maximum writing
#define OW_COUNT	6		// The number of times of additional writing
#define BLK_MAX	16		// EB0..EB15
#define FLMCR1_MAXBLOCK 7	//EB0..7 controlled by FLMCR1


/** FLMCRx bit defines */
#define FLMCR_FWE	0x80
#define FLMCR_FLER	0x80
#define FLMCR_SWE	0x40
#define FLMCR_ESU	0x20
#define FLMCR_PSU	0x10
#define FLMCR_EV	0x08
#define FLMCR_PV	0x04
#define FLMCR_E	0x02
#define FLMCR_P	0x01



#ifdef ASM_IMPLEM
/* probably dead-end attempt at porting the Renesas FDT code.
Size with no implems: 3392	1024	536 => 4952
Size with Erase asm implem: 4024	1024	552 => 5600, delta = 648B
*/

/** Common timing constants */
const u32 SWES_W = WAITN_CALCN(1);
const u32 SWEC_W = WAITN_CALCN(100);
const u32 DLCH_W = WAITN_CALCN(2);

/** Erase timing constants */
const u32 ESUS_W = WAITN_CALCN(100);
const u32 ESUC_W = WAITN_CALCN(10);
const u32 ES_W = WAITN_CALCN(10000UL);
const u32 EC_W = WAITN_CALCN(10);
const u32 EVS_W = WAITN_CALCN(20);
const u32 EVC_W = WAITN_CALCN(4);

/** Write timing constants */
const u32 PSUS_W = WAITN_CALCN(50);
const u32 PSUC_W = WAITN_CALCN(5);
const u32 P10S_W = WAITN_CALCN(10);
const u32 P30S_W = WAITN_CALCN(30);
const u32 P200S_W = WAITN_CALCN(200);
const u32 PC_W = WAITN_CALCN(5);
const u32 PVS_W = WAITN_CALCN(4);
const u32 PVC_W = WAITN_CALCN(2);


/** low-level asm function protos */
u32 block_erase(u32 blockno);

#else
/* Regular implem */

const u32 fblocks[] = {
	0x00000000,
	0x00001000,
	0x00002000,
	0x00003000,
	0x00004000,
	0x00005000,
	0x00006000,
	0x00007000,
	0x00008000,
	0x00010000,
	0x00020000,
	0x00030000,
	0x00040000,
	0x00050000,
	0x00060000,
	0x00070000,
	0x00080000		//last one just for delimiting the last block
};

#endif // ASM_IMPLEM




static bool reflash_enabled = 0;	//global flag to protect flash, see platf_flash_enable()


static volatile u8 *pFLMCR;	//will point to FLMCR1 or FLMCR2 as required
static volatile u8 *pEBR;		//similar idea, for EBR1 / EBR2


/** spin for <loops> .
 * Constants should be calculated at compile-time.
 */
#define WAITN_TCYCLE	4	//clock cycles per loop

static void waitn(unsigned loops) {
	u32 tmp;
	asm volatile ("0: dt %0":"=r"(tmp):"0"(loops):"cc");
	asm volatile ("bf 0b");
}



/** Check FWE and FLER bits
 * ret 1 if ok
 */
static bool fwecheck(void) {
	if (!FLASH.FLMCR1.BIT.FWE) return 0;
	if (FLASH.FLMCR2.BIT.FLER) return 0;
	return 1;
}

/** Set SWE bit and wait */
static void sweset(void) {
	*pFLMCR |= FLMCR_SWE;
	waitn(TSSWE);
	return;
}

/** Clear SWE bit and wait */
static void sweclear(void) {
	*pFLMCR &= ~FLMCR_SWE;
	waitn(TCSWE);
}

/** Erase verification
 * Assumes pFLMCR is set, of course
 * ret 1 if ok
 */
static bool ferasevf(unsigned blockno) {
	bool rv = 1;
	volatile u32 *cur, *end;

	cur = (volatile u32 *) fblocks[blockno];
	end = (volatile u32 *) fblocks[blockno + 1];

	for (; cur < end; cur++) {
		*pFLMCR |= FLMCR_EV;
		waitn(TSEV);
		*cur = 0xFFFFFFFF;
		waitn(TSEVR);
		if (*cur != 0xFFFFFFFF) {
			rv = 0;
			break;
		}
	}
	*pFLMCR &= ~FLMCR_EV;
	waitn(TCEV);

	return rv;
}



/* pFLMCR and pEBR must be set;
 * blockno validated <= 15 of course
 */
static void ferase(unsigned blockno) {
	unsigned bitsel;

	bitsel = 1;
	blockno &= 0x07;	//keep 3 bits for EB0..7 or EB8..15
	while (blockno) {
		bitsel <<= 1;
		blockno -= 1;
	}
	*pEBR = bitsel;

	WDT.WRITE.TCSR = WDT_TCSR_STOP;	//this also clears TCNT
	WDT.WRITE.TCSR = WDT_TCSR_START;

	*pFLMCR |= FLMCR_ESU;
	waitn(TSESU);
	*pFLMCR |= FLMCR_E;	//start Erase pulse
	waitn(TSE);
	*pFLMCR &= ~FLMCR_E;	//stop pulse
	waitn(TCE);
	*pFLMCR &= ~FLMCR_ESU;
	waitn(TCESU);

	WDT.WRITE.TCSR = WDT_TCSR_STOP;

	*pEBR = 0;

}



//TODO : give sane values to these & other errors
#define FL_ERROR 0x80

bool platf_flash_init(u8 *err) {
	reflash_enabled = 0;

	//Check FLER
	if (!fwecheck()) {
		*err = FL_ERROR;
		return 0;
	}

	/* suxxess ! */
	return 1;

}


void platf_flash_unprotect(void) {
	reflash_enabled = 1;
}


//same error codes as 180nm, for convenience maybe
#define PFEB_BADBLOCK (0x84 | 0x00)
#define PFEB_VERIFAIL (0x84 | 0x01)
uint32_t platf_flash_eb(unsigned blockno) {
	unsigned count;

	if (blockno >= BLK_MAX) return PFEB_BADBLOCK;
	if (!reflash_enabled) return 0;

#ifdef ASM_IMPLEM
	if (block_erase(blockno)) return 0;
	return -1;
#else

	if (blockno > FLMCR1_MAXBLOCK) {
		pFLMCR = &FLASH.FLMCR1.BYTE;
		pEBR = &FLASH.EBR1.BYTE;
	} else {
		pFLMCR = &FLASH.FLMCR2.BYTE;
		pEBR = &FLASH.EBR2.BYTE;
	}

	if (!fwecheck()) {
		return -1;
	}

	sweset();
	WDT.WRITE.TCSR = WDT_TCSR_STOP;
	WDT.WRITE.RSTCSR = WDT_RSTCSR_SETTING;

	//XXX etc
	if (ferasevf(blockno)) {
		//already blank
		sweclear();
		return 0;
	}


	for (count = 0; count < MAX_ET; count++) {
		ferase(blockno);
		if (ferasevf(blockno)) {
			sweclear();
			return 0;
		}
	}
	/* haven't managed to get a succesful ferasevf() : badexit */
	sweclear();
	return -1;
#endif
}


/** ret 0 if ok,
 * assumes params are ok, and that block was already erased
 */
uint32_t flash_write128(uint32_t dest, uint32_t src) {
	(void) dest;
	(void) src;
	return -1;
}


//defined like 180nm code
#define PFWB_OOB (0x88 | 0x00)		//dest out of bounds
#define PFWB_MISALIGNED (0x88 | 0x01)	//dest not on 128B boundary
#define PFWB_LEN (0x88 | 0x02)		//len not multiple of 128
#define PFWB_VERIFAIL (0x88 | 0x03)	//post-write verify failed

uint32_t platf_flash_wb(uint32_t dest, uint32_t src, uint32_t len) {

	if (dest > FL_MAXROM) return PFWB_OOB;
	if (dest & 0x7F) return PFWB_MISALIGNED;	//dest not aligned on 128B boundary
	if (len & 0x7F) return PFWB_LEN;	//must be multiple of 128B too

	while (len) {
		uint32_t rv = 0;

		if (reflash_enabled) {
			rv = flash_write128(dest, src);
		}
		if (rv) {
			return rv;	//TODO : tweak into valid NRC
		}

		if (memcmp((void *)dest, (void *)src, 128) != 0) return PFWB_VERIFAIL;

		dest += 128;
		src += 128;
		len -= 128;
	}
	return 0;
}
