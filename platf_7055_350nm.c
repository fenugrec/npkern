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

const struct flashblock fblocks[] = {
	{0x00000000,	0x00001000},
	{0x00001000,	0x00001000},
	{0x00002000,	0x00001000},
	{0x00003000,	0x00001000},
	{0x00004000,	0x00001000},
	{0x00005000,	0x00001000},
	{0x00006000,	0x00001000},
	{0x00007000,	0x00001000},
	{0x00008000,	0x00008000},
	{0x00010000,	0x00010000},
	{0x00020000,	0x00010000},
	{0x00030000,	0x00010000},
	{0x00040000,	0x00010000},
	{0x00050000,	0x00010000},
	{0x00060000,	0x00010000},
	{0x00070000,	0x00010000},
};

#define FL_NUMBLOCKS 16		//EB0..15

//Assume 40MHz clock. Some critical timing depends on this being true
#define CPUFREQ	(40)



static bool reflash_enabled = 0;	//global flag to protect flash, see platf_flash_enable()


/*
 *
 *
 */
bool platf_flash_init(u8 *err) {
	reflash_enabled = 0;

	//Check FLER
	//*err = asdf

	/* suxxess ! */
	return 1;

badexit:
	return 0;
}


void platf_flash_unprotect(void) {
	reflash_enabled = 1;
}


//same error codes as 180nm, for convenience maybe
#define PFEB_BADBLOCK (0x84 | 0x00)
#define PFEB_VERIFAIL (0x84 | 0x01)
uint32_t platf_flash_eb(unsigned blockno) {

	if (blockno >= FL_NUMBLOCKS) return PFEB_BADBLOCK;
	if (!reflash_enabled) return 0;

	return 0;
}


/** ret 0 if ok,
 * assumes params are ok, and that block was already erased
 */
uint32_t flash_write128(uint32_t dest, uint32_t src) {
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
