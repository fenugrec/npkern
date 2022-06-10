/* external WDT functions specific to Nissan ECUs
 *
 * (c) fenugrec 2022
 * GPLv3
 *
 */

#include <stdint.h>

#include "mfg.h"
#include "platf.h"
#include "stypes.h"
#include "wdt.h"


/**** "preload" info struct filled before calling RAMjump. This probably varies but seems typical ? *****/
struct rj_preload {
	uint16_t flag0;	// ?
	uint16_t wdt_pin;	//  pin # mask for PxDR
	uint16_t s36k2_H;
	uint16_t s36k2_L;
	uint16_t wdt_portH;	//&PxDR>>16
	uint16_t wdt_portL;	//&PxDR & 0xFFFF
};


/** parse "preload" info 
 */
void init_mfg(void) {
	struct rj_preload *rjp = (struct rj_preload *)RAMJUMP_PRELOAD_META;
	/* get pin info */
	wdt_dr = (u16 *) ((rjp->wdt_portH << 16) | (rjp->wdt_portL));
	wdt_pin = rjp->wdt_pin;
}

