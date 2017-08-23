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
#include "reg_defines/7055_7058_180nm.h"	//io peripheral regs etc

#include <string.h>	//memcmp
#include "stypes.h"
#include "platf.h"
#include "iso_cmds.h"
#include "npk_errcodes.h"

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
	0x00020000,
	0x00040000,
	0x00060000,
	0x00080000,
	0x000A0000,
	0x000C0000,
	0x000E0000,
	0x00100000,	/* end delimiter */
};

#elif defined(SH7055_18)

#define FL_ERASE_BASE	0xFFFF7000
#define FL_WRITE_BASE	0xFFFF7800

#define FL_MAXROM	(512*1024UL - 1UL)

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
	0x00080000,	/* end delimiter */
};

#else
#error No target specified !

#endif


#define FL_FPEFEQ	(40 * 100)
#define FL_ERASEBLOCKS	15	//EB0...EB15; see DS





/* Function pointers to builtin microcodes, once "downloaded" to RAM.
 * These protos assume that gcc follows this part of the renesas calling convention :
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






static bool reflash_enabled = 0;	//global flag to protect flash, see platf_flash_enable()


/*
 *
 * Copy + initialize mcu's builtin write + erase functions.
 *
 */
bool platf_flash_init(u8 *err) {
	volatile u8 *DPFR;
	uint32_t FPFR;
	int imask_save;
	void *vbr_save;

	reflash_enabled = 0;

	//Check 180nm vs 350nm : on 350nm, FKEY is undefined; 180nm has an 8bit RW register
	FLASH.FKEY = 0x33;
	if (FLASH.FKEY != 0x33) {
		*err = PF_SILICON;
		return 0;
	}

	if (FLASH.FCCS.BYTE != 0x80) {
		//FLER bit set, or FWE pin bad level
		*err = SID34_BADFCCS;
		return 0;
	}

	if (FLASH.RAMER.BIT.RAMS) {
		// RAMER enabled; can't proceed
		*err = SID34_BADRAMER;
		return 0;
	}

	/*****  copy Erase prog */
	DPFR = (void *) FL_ERASE_BASE;
	*DPFR = 0xFF;	//preload ret value to Error state

	FLASH.FPCS.BYTE = 0;
	FLASH.FECS.BYTE = 1;	//select Erase microcode
	FLASH.FTDAR.BYTE = FTDAR_ERASE;

	imask_save = get_imask();
	vbr_save = get_vbr();
	set_imask(0x0F);
	set_vbr(0);	/* Copying takes about 75us per microcode, so no problem for WDT disruption */

	FLASH.FKEY = 0xA5;
	FLASH.FCCS.BIT.SCO = 1;	//start copying
	nop();
	nop();
	nop();
	nop();
	if (*DPFR) {
		//SCO download error.
		*err = SID34_BADDL_ERASE;
		goto badexit;
	}

	/******** copy Write prog */
	DPFR = (void *) FL_WRITE_BASE;
	*DPFR = 0xFF;	//preload ret value to Error state
	FLASH.FECS.BYTE = 0;
	FLASH.FPCS.BYTE = 1;	//select Write microcode
	FLASH.FTDAR.BYTE = FTDAR_WRITE;
	FLASH.FKEY = 0xA5;
	FLASH.FCCS.BIT.SCO = 1;
	nop();
	nop();
	nop();
	nop();
	if (*DPFR) {
		//SCO download error.
		*err = SID34_BADDL_WRITE;
		goto badexit;
	}

	/***** Restore ints + VBR ****/
	set_vbr(vbr_save);
	set_imask(imask_save);
	FLASH.FKEY = 0;


	/***** initialize both microcodes */
	FPFR = fl_erase_init(FL_FPEFEQ, 0);
	if (FPFR) {
		*err = SID34_BADINIT_ERASE;
		goto badexit;
	}
	FPFR = fl_write_init(FL_FPEFEQ, 0);
	if (FPFR) {
		*err = SID34_BADINIT_WRITE;
		goto badexit;
	}

	/* suxxess ! */
	return 1;

badexit:
	set_vbr(vbr_save);
	set_imask(imask_save);
	return 0;
}


void platf_flash_unprotect(void) {
	reflash_enabled = 1;
}


uint32_t platf_flash_eb(unsigned blockno) {
	uint32_t FPFR;

	if (blockno > FL_ERASEBLOCKS) return PFEB_BADBLOCK;
	if (!reflash_enabled) return 0;

	FLASH.FKEY = 0x5A;
	FPFR = fl_erase(blockno);
	if (FPFR) {
		FLASH.FKEY = 0;
		return ((FPFR & 0xFF) | 0x80);
	}
	FLASH.FKEY = 0;
#ifdef POSTERASE_VERIFY
	uint32_t vcur = fblocks[blockno];
	uint32_t end = fblocks[blockno + 1];
	for (; vcur < end; vcur += 4) {
		if (*(uint32_t *) vcur != 0xFFFFFFFF) return PFEB_VERIFAIL;
	}
#endif
	return 0;
}


/** ret 0 if ok, FPFR value if failed
 * assumes params are ok, and that block was already erased
 */
uint32_t flash_write128(uint32_t dest, uint32_t src) {
	uint32_t FPFR;

	FLASH.FKEY = 0x5A;
	FPFR = fl_write(src, dest);
	FLASH.FKEY = 0;
	return FPFR;
}


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
			return (rv & 0xFF) | 0x80;	//tweak into valid NRC
		}

		if (memcmp((void *)dest, (void *)src, 128) != 0) return PFWB_VERIFAIL;

		dest += 128;
		src += 128;
		len -= 128;
	}
	return 0;
}
