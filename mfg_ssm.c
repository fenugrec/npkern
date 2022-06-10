/* functions specific to Subaru ECUs
 *
 * (c) fenugrec 2022
 * (c) rimwall 2022
 * GPLv3
 *
 */

#include <stdint.h>

#include "mfg.h"
#include "platf.h"
#include "wdt.h"



void init_mfg(void) {
	wdt_dr = &PB.DR.WORD;  //manually assign these values, not elegant but will do for now
	wdt_pin = 0x8000;

	// set PD8 to bring FWE high to prepare for erasing or flashing
	PD.DR.WORD |= 0x0100;
}
