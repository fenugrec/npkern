/* external WDT functions; common code for all platforms
 *
 * (c) fenugrec 2022
 * GPLv3
 *
 * there is an IC outside the SH705x that monitors a certain pin. This pin needs to be toggled
 * at a certain frequency, otherwise the external IC will reset the cpu.
 */

#include <stdint.h>

#include "stypes.h"
#include "wdt.h"


volatile u16 *wdt_dr;	//such as &PLDR
u16 wdt_pin;	//mask for PxDR



/** toggle WDT pin */
void wdt_tog(void) {
#ifdef DIAG_TAINTWDT
	/* for debugging purposes, generate a recognizable pattern :
	 * skip one toggle every 16 periods */
	static int ic = 0;
	ic += 1;
	if (ic & 0x0f) {
		*wdt_dr ^= wdt_pin;
	}
#else
	*wdt_dr ^= wdt_pin;
#endif
	return;
}

