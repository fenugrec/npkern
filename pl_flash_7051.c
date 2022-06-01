/* platform-specific code (see platf.h)
 * Implements the reflashing back-end commands for older SH7051.
 */

/* (c) copyright fenugrec 2016, a33b 2020
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

/* See platf_7055... for general notes
This was hacked together by a33b
*/


#include "functions.h"
#include "extra_functions.h"
#include "reg_defines/7051.h"	//io peripheral regs etc

#include <string.h>	//memcpy
#include "stypes.h"
#include "platf.h"
#include "iso_cmds.h"
#include "npk_errcodes.h"

/*********  Reflashing defines
 *
 * This is for SH7051 and assumes this RAM map (see .ld file)
 *
 * - stack @ 0xFFFF FFFC  (growing downwards)
 * - kernel @ 0xFFFF D840, this leaves just less than ~10k for both kernel + stack
 */


#ifndef SH7051
#error Wrong target specified !
#endif

#define FL_MAXROM	(256*1024UL - 1UL)


/********** Timing defs
*/

//20MHz clock. Some critical timing depends on this being true,
//WDT stuff in particular isn't macro-fied
#define CPUFREQ	(20)
#define ATUPRESCALER (32)

#define WDT_RSTCSR_SETTING 0x5A4F	//reset if TCNT overflows
#define WDT_TCSR_ESTART (0xA578 | 0x06)	//write value to start with 1:4096 div (52.4 ms @ 20MHz), for erase runaway
#define WDT_TCSR_WSTART (0xA578 | 0x05)	//write value to start with 1:1024 div (13.1 ms @ 20MHz), for write runaway
#define WDT_TCSR_STOP 0xA558	//write value to stop WDT count

#define WAITN_TCYCLE 4		/* clock cycles per loop, see asm */
#define WAITN_CALCN(usec) (((usec) * CPUFREQ / WAITN_TCYCLE) + 1)
#define WAITN_TSE_CALCN(usec) (((usec) * CPUFREQ / ATUPRESCALER))

/** Common timing constants */
#define TSSWE	WAITN_CALCN(10)
#define TCSWE	WAITN_CALCN(100)  //Not in Hitachi datasheet, but shouldn't hurt

/** Erase timing constants */
#define TSESU	WAITN_CALCN(200)
#define TSE	WAITN_TSE_CALCN(5000UL) //need to toggle ext WDT pin during wait
#define TCE	WAITN_CALCN(10)
#define TCESU	WAITN_CALCN(10)
#define TSEV	WAITN_CALCN(10)	/******** Renesas has 20 for this !?? */
#define TSEVR	WAITN_CALCN(2)
#define TCEV	WAITN_CALCN(5)


/** Write timing constants */
#define TSPSU	WAITN_CALCN(300) //Datasheet has 50, F-ZTAT has 300
#define TSP500	WAITN_CALCN(500)
#define TCP		WAITN_CALCN(10)
#define TCPSU	WAITN_CALCN(10)
#define TSPV	WAITN_CALCN(10) //Datasheet has 4, F-ZTAT has 10
#define TSPVR	WAITN_CALCN(5) //Datasheet has 2, F-ZTAT has 5
#define TCPV	WAITN_CALCN(5) //Datasheet has 4, F-ZTAT has 5


/** FLASH constants */
#define MAX_ET	61		// The number of times of the maximum erase
#define MAX_WT	400		// The number of times of the maximum writing
#define BLK_MAX	12		// EB0..EB11
#define FLMCR2_BEGIN 0x20000 //20000 - 3FFFF controlled by FLMCR2


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


const u32 fblocks[] = {
	0x00000000,
	0x00008000,
	0x00010000,
	0x00018000,
	0x00020000,
	0x00028000,
	0x00030000,
	0x00038000,
	0x0003F000,
	0x0003F400,
	0x0003F800,
	0x0003FC00,
	0x00040000,	//last one just for delimiting the last block
};




static bool reflash_enabled = 0;	//global flag to protect flash, see platf_flash_enable()

static volatile u8 *pFLMCR;	//will point to FLMCR1 or FLMCR2 as required


/** spin for <loops> .
 * Constants should be calculated at compile-time.
 */
#define WAITN_TCYCLE	4	//clock cycles per loop

static void waitn(unsigned loops) {
	u32 tmp;
	asm volatile ("0: dt %0":"=r"(tmp):"0"(loops):"cc");
	asm volatile ("bf 0b");
}

//implemented in main.c
void wdt_tog(void);


static void manual_wdt(void) {
    if (CMT1.CMCNT >= (WDT_MAXCNT - 50)) { // -50 so we can hopefully be close enough after waitn calcs
		wdt_tog();
		CMT1.CMCNT = 0;
		CMT1.CMCSR.BIT.CMF = 0;
    }
}

static void waitn_tse(void) {
	u32 start = ATU0.TCNT;
	while ((ATU0.TCNT - start) < TSE)
	{
		manual_wdt();
	}
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
	CMT1.CMCSR.BIT.CMIE = 0;	// Disable interrupt on 7051 for erase/write
	FLASH.FLMCR1.BIT.SWE = 1;
	waitn(TSSWE);
	return;
}

/** Clear SWE bit and wait */
static void sweclear(void) {
	FLASH.FLMCR1.BIT.SWE = 0;
	CMT1.CMCSR.BIT.CMIE = 1;	// Re-enable interrupt
	waitn(TCSWE);
}


/*********** Erase ***********/

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
		manual_wdt();
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



/* pFLMCR must be set;
 * blockno validated <= 11 of course
 */
static void ferase(unsigned blockno) {
	unsigned bitsel;

	bitsel = 1;
	while (blockno) {
		bitsel <<= 1;
		blockno -= 1;
	}

	FLASH.EBR2.BYTE = 0;	//to ensure we don't have > 1 bit set simultaneously
	FLASH.EBR1.BYTE = bitsel & 0x0F;	//EB0..3
	FLASH.EBR2.BYTE = (bitsel >> 4) & 0xFF;	//EB4..11

	WDT.WRITE.TCSR = WDT_TCSR_STOP;	//this also clears TCNT
	WDT.WRITE.TCSR = WDT_TCSR_ESTART;
	manual_wdt();

	*pFLMCR |= FLMCR_ESU;
	waitn(TSESU);
	*pFLMCR |= FLMCR_E;	//start Erase pulse
	waitn_tse();
	*pFLMCR &= ~FLMCR_E;	//stop pulse
	waitn(TCE);
	*pFLMCR &= ~FLMCR_ESU;
	waitn(TCESU);

	manual_wdt();
	WDT.WRITE.TCSR = WDT_TCSR_STOP;

	FLASH.EBR1.BYTE = 0;
	FLASH.EBR2.BYTE = 0;

}



uint32_t platf_flash_eb(unsigned blockno) {
	unsigned count;

	if (blockno >= BLK_MAX) return PFEB_BADBLOCK;
	if (!reflash_enabled) return 0;

	if (fblocks[blockno] >= FLMCR2_BEGIN) {
		pFLMCR = &FLASH.FLMCR2.BYTE;
	} else {
		pFLMCR = &FLASH.FLMCR1.BYTE;
	}

	if (!fwecheck()) {
		return PF_ERROR;
	}

	sweset();
	WDT.WRITE.TCSR = WDT_TCSR_STOP;
	WDT.WRITE.RSTCSR = WDT_RSTCSR_SETTING;

	/* XXX is this pre-erase verify really necessary ?? DS doesn't say so;
	 * FDT example has this, and Nissan kernel doesn't
	 */
#if 0
	if (ferasevf(blockno)) {
		//already blank
		sweclear();
		return 0;
	}
#endif


	for (count = 0; count < MAX_ET; count++) {
		ferase(blockno);
		if (ferasevf(blockno)) {
			sweclear();
#if 0
			if (!fwecheck()) {
				return PF_ERROR_AFTERASE;
			}
#endif
			return 0;
		}
	}

	/* haven't managed to get a succesful ferasevf() : badexit */
	sweclear();
	return PFEB_VERIFAIL;

}



/*********** Write ***********/

/** Copy 32-byte chunk + apply write pulse for tsp=500us
 */
static void writepulse(volatile u8 *dest, u8 *src, unsigned tsp) {
	unsigned uim;
	u32 cur;

	//can't use memcpy because these must be byte transfers
	for (cur = 0; cur < 32; cur++) {
		dest[cur] = src[cur];
	}

	uim = imask_savedisable();

	WDT.WRITE.TCSR = WDT_TCSR_STOP;
	WDT.WRITE.TCSR = WDT_TCSR_WSTART;
	manual_wdt();

	*pFLMCR |= FLMCR_PSU;
	waitn(TSPSU);		//F-ZTAT has 300 here
	*pFLMCR |= FLMCR_P;
	waitn(tsp);
	*pFLMCR &= ~FLMCR_P;
	waitn(TCP);
	*pFLMCR &= ~FLMCR_PSU;
	waitn(TCPSU);
	WDT.WRITE.TCSR = WDT_TCSR_STOP;

	imask_restore(uim);
}


/** ret 0 if ok, NRC if error
 * assumes params are ok, and that block was already erased
 */
static u32 flash_write32(u32 dest, u32 src_unaligned) {
	u8 src[32] __attribute ((aligned (4)));	// aligned copy of desired data
	u8 reprog[32] __attribute ((aligned (4)));	// retry / reprogram data

	unsigned n;
	bool m;
	u32 rv;

	if (dest < FLMCR2_BEGIN) {
		pFLMCR = &FLASH.FLMCR1.BYTE;
	} else {
		pFLMCR = &FLASH.FLMCR2.BYTE;
	}

#if 0
	if (!fwecheck()) {
		return PF_ERROR_B4WRITE;
	}
#endif

	memcpy(src, (void *) src_unaligned, 32);
	memcpy(reprog, (void *) src, 32);

	sweset();
	WDT.WRITE.TCSR = WDT_TCSR_STOP;
	WDT.WRITE.RSTCSR = WDT_RSTCSR_SETTING;

	for (n=1; n < MAX_WT; n++) {
		unsigned cur;

		m = 0;
		manual_wdt();
		
		//1) write (latch) to flash, with 500us pulse
		writepulse((volatile u8 *)dest, reprog, TSP500);
		
		manual_wdt();
#if 0
		if (!fwecheck()) {
			return PF_ERROR_AFTWRITE;
		}
#endif
		//2) Program verify
		*pFLMCR |= FLMCR_PV;
		waitn(TSPV);	//F-ZTAT has 10 here

		for (cur = 0; cur < 32; cur += 4) {
			u32 verifdata;
			u32 srcdata;

			//dummy write 0xFFFFFFFF
			*(volatile u32 *) (dest + cur) = (u32) -1;
			waitn(TSPVR);	//F-ZTAT has 5 here

			verifdata = *(volatile u32 *) (dest + cur);
			srcdata = *(u32 *) (src + cur);

			manual_wdt();
#if 0
			if (!fwecheck()) {
				return PF_ERROR_VERIF;
			}
#endif

			if (verifdata != srcdata) {
				//mismatch:
				m = 1;
			}

			if (srcdata & ~verifdata) {
				//wanted '1' bits, but somehow got '0's : serious error
				rv = PFWB_VERIFAIL;
				goto badexit;
			}
			//compute reprogramming data. This fits with my reading of both the DS and the FDT code,
			//but Nissan proceeds differently
            * (u32 *) (reprog + cur) = srcdata | ~verifdata;
		}	//for (program verif)

		*pFLMCR &= ~FLMCR_PV;
		waitn(TCPV);	//F-ZTAT has 5 here

		if (!m) {
			//success
			sweclear();
			return 0;
		}

	}	//for (n < 400)

	//failed, max # of retries
	rv = PFWB_MAXRET;
badexit:
	sweclear();
	return rv;
}


uint32_t platf_flash_wb(uint32_t dest, uint32_t src, uint32_t len) {

	if (dest > FL_MAXROM) return PFWB_OOB;
	if (dest & 0x1F) return PFWB_MISALIGNED;	//dest not aligned on 32B boundary
	if (len & 0x1F) return PFWB_LEN;	//must be multiple of 32B too

	if (!reflash_enabled) return 0;	//pretend success

	while (len) {
		uint32_t rv = 0;

		rv = flash_write32(dest, src);

		if (rv) {
			return rv;
		}

		dest += 32;
		src += 32;
		len -= 32;
	}
	return 0;
}


/*********** init, unprotect ***********/


bool platf_flash_init(u8 *err) {
	reflash_enabled = 0;

	//Check FLER
	if (!fwecheck()) {
		*err = PF_ERROR;
		return 0;
	}

	/* suxxess ! */
	return 1;

}


void platf_flash_unprotect(void) {
	reflash_enabled = 1;
}

