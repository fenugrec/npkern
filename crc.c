/* CRC implementations */


#include <stdbool.h>
#include <stdint.h>
#include "stypes.h"

//#define CRC16	0xC86C	//"baicheva00"
#define CRC16	0xBAAD	//koopman, 2048bits (256B)
//#define CRC16	0xa001	//common CRC16 (winhex)


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
		u8 nextval;

		nextval = *data++;
		tmp =  crc       ^ nextval;
		crc = (crc >> 8) ^ crc_tab16[ tmp & 0xff ];
	#if 0
		// not worth it : a lot more code, and only 0.5cy/B savings
		nextval = *data++;
		tmp =  crc       ^ nextval;
		crc = (crc >> 8) ^ crc_tab16[ tmp & 0xff ];
		siz -= 2;
	#else
		siz -= 1;
	#endif
	}

	return crc;
}
