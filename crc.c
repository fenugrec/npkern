/* test different CRC32 implementations */
/* Adapted from http://www.hackersdelight.org/hdcodetxt/crc.c.txt
 */

#include <stdint.h>
#include "stypes.h"

#define CRCPOLY 0xEDB88320


/* approx 80 cycles per byte, codesize = 0x30 */
u32 crc32b(u8 *message, u32 siz) {
	int i, j;
	u32 crc, mask;


	i = 0;
	crc = 0xFFFFFFFF;
	while (siz) {
		u8 byte;
		byte = message[i];            // Get next byte.
		crc = crc ^ byte;
		for (j = 7; j >= 0; j--) {    // Do eight times.
			mask = -(crc & 1);
			crc = (crc >> 1) ^ (CRCPOLY & mask);
		}
		i = i + 1;
		siz -= 1;
	}
	return ~crc;
}


/*  ~ 88 cycles/byte; siz=0x8c*/
u32 crc32g(u8 *message, u32 siz) {
	int i, j;
	int32_t crc;
	u8 byte;
	u32 c;
	const u32 g0 = CRCPOLY, g1 = g0 >> 1,
		g2 = g0 >> 2,    g3 = g0 >> 3;

	i = 0;
	crc = 0xFFFFFFFF;
	while (siz) {
		byte = message[i];                // Get next byte.
		crc = crc ^ byte;
		for (j = 1; j >= 0; j--) {        // Do two times.
			c = ((crc<<31>>31) & g3) ^ ((crc<<30>>31) & g2) ^
			((crc<<29>>31) & g1) ^ ((crc<<28>>31) & g0);
			crc = ((u32)crc >> 4) ^ c;
		}
		i = i + 1;
		siz -= 1;
	}
	return ~crc;
}



/*** designed for little-endian ! (orig name "crc32cx" */
/* BUT 8.25 cy/byte + 32 ovh; siz = 0x94; tablesiz=1kB */
u32 crc_be_8bt(const u32 *message, u32 siz) {
	int j;
	u32 byte;
	u32 crc;
	static u32 table[256];

	/* Set up the table, if necessary. */

	if (table[1] == 0) {
		for (byte = 0; byte <= 255; byte++) {
			crc = byte;
			for (j = 7; j >= 0; j--) {    // Do eight times.
				u32 mask;
				mask = -(crc & 1);
				crc = (crc >> 1) ^ (CRCPOLY & mask);
			}
			table[byte] = crc;
		}
	}

	/* Through with table setup, now calculate the CRC. */

	crc = 0xFFFFFFFF;
	//siz /= 4;	//# of u32 words
	while (siz > 0) {
		u32 word = *message;
		crc = crc ^ word;
		crc = (crc >> 8) ^ table[crc & 0xFF];
		crc = (crc >> 8) ^ table[crc & 0xFF];
		crc = (crc >> 8) ^ table[crc & 0xFF];
		crc = (crc >> 8) ^ table[crc & 0xFF];
		message += 1;
		siz -= 4;
	}
	return ~crc;
}


/* version for smaller table (64B); per 4bit nib . ~ 21.5 cy/byte, sizeof(func)=0x78  */
u32 crc_be_4bt(const u32 *message, u32 siz) {
	int j;
	u32 byte;
	u32 crc;
	static u32 table[16];

	/* Set up the table, if necessary. */

	if (table[1] == 0) {
		for (byte = 0; byte < 16; byte++) {
			crc = byte;
			for (j = 4; j >= 0; j--) {
				u32 mask;
				mask = -(crc & 1);
				crc = (crc >> 1) ^ (CRCPOLY & mask);
			}
			table[byte] = crc;
		}
	}

	/* Through with table setup, now calculate the CRC. */

	crc = 0xFFFFFFFF;
	while (siz > 0) {
		u32 word = *message;
		crc = crc ^ word;
		int i;
		#if 1
		for (i = 0; i < 8; i++) {
			crc = (crc >> 4) ^ table[crc & 0x0F];
		}
		#else	//code = 0x5c bigger, but 17cy/B
		crc = (crc >> 4) ^ table[crc & 0x0F];
		crc = (crc >> 4) ^ table[crc & 0x0F];
		crc = (crc >> 4) ^ table[crc & 0x0F];
		crc = (crc >> 4) ^ table[crc & 0x0F];
		crc = (crc >> 4) ^ table[crc & 0x0F];
		crc = (crc >> 4) ^ table[crc & 0x0F];
		crc = (crc >> 4) ^ table[crc & 0x0F];
		crc = (crc >> 4) ^ table[crc & 0x0F];
		#endif
		message += 1;
		siz -= 4;
	}
	return ~crc;
}
