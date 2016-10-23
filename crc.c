/* test different CRC32 implementations */


#include <stdbool.h>
#include <stdint.h>
#include "stypes.h"

//#define CRC16	0xC86C	//"baicheva00"
#define CRC16	0xBAAD	//koopman, 2048bits (256B)
//#define CRC16	0xa001	//common CRC16 (winhex)

#define CRCPOLY 0xEDB88320


/*** CRC16 implementation adapted from Lammert Bies
 * https://www.lammertbies.nl/comm/info/crc-calculation.html
 */
static bool crc_tab16_init = 0;
static u16 crc_tab16[256];

static void init_crc16_tab( void ) {
	u32 i, j;
	u16 crc, c;

	for (i=0; i<256; i++) {
		crc = 0;
		c   = (u16) i;

		for (j=0; j<8; j++) {
			if ( (crc ^ c) & 0x0001 ) crc = ( crc >> 1 ) ^ CRC16;
			else                      crc =   crc >> 1;
			c = c >> 1;
		}
		crc_tab16[i] = crc;
	}

	crc_tab16_init = 1;

}  /* init_crc16_tab */


/* 12 cy/byte; codesize = 0x78; tablesiz = 512B */
u16 crc16(const u8 *data, u32 siz) {
	u16 crc;

	if ( ! crc_tab16_init ) init_crc16_tab();

	crc = 0;
	while (siz > 0) {
		u16 tmp;
		u8 nextval = *data;

		tmp =  crc       ^ nextval;
		crc = (crc >> 8) ^ crc_tab16[ tmp & 0xff ];

		data += 1;
		siz -= 1;
	}

	return crc;
}

/* A few adapted from http://www.hackersdelight.org/hdcodetxt/crc.c.txt
 */


/* approx 80 cycles per byte, codesize = 0x30 */
u32 crc32b(const u8 *message, u32 siz) {
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

