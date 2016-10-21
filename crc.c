/* test different CRC32 implementations */
/* Adapted from http://www.hackersdelight.org/hdcodetxt/crc.c.txt
 */

#include <stdint.h>
#include "stypes.h"

/* approx 71 cycles per byte, codesize = 0x2c */
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
			crc = (crc >> 1) ^ (0xEDB88320 & mask);
		}
		i = i + 1;
		siz -= 1;
	}
	return ~crc;
}


/*  ~ 39 cycles/byte; siz=0x8c*/
u32 crc32g(u8 *message, u32 siz) {
	int i, j, crc;
	u8 byte;
	u32 c;
	const u32 g0 = 0xEDB88320, g1 = g0 >> 1,
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
