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



#include "stypes.h"

#include "functions.h"	//for set_imask etc

#include "platf.h"
#include "cmd_parser.h"
   

/**** "preload" info struct filled before calling RAMjump. This probably varies but seems typical ? *****/
struct rj_preload {
	uint16_t flag0;	// ?
	uint16_t wdt_pin;	//  pin # mask for PxDR
	uint16_t s36k2_H;
	uint16_t s36k2_L;
	uint16_t wdt_portH;	//&PxDR>>16
	uint16_t wdt_portL;	//&PxDR & 0xFFFF
};

u16 *wdt_dr;	//such as &PLDR
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

#if 0
/** poll timer to generate WDT pulse */
void do_wdt(void) {
	if (ATU1.TCNTB > (WDT_MAXCNT + 100)) {
		wdt_tog();
		ATU1.TCNTB = 0;
		ATU1.TSRB.BIT.CMF = 0;	//TCNT1B compare match
	}
	return;
}
#endif



/* force reset by supervisor */
void die(void) {
	set_imask(0x0F);
	while (1) {}
	return;
}


void main(void) {
	struct rj_preload *rjp = (struct rj_preload *)RAMJUMP_PRELOAD_META;

	set_imask(0x0F);	// disable interrupts (mask = b'1111)

	init_platf();

	/* parse preload struct to get wdt info */
	wdt_dr = (u16 *) ((rjp->wdt_portH << 16) | (rjp->wdt_portL));
	wdt_pin = rjp->wdt_pin;

	init_wdt();

	/* and lower prio mask to let WDT run */
	set_imask(0x07);

	cmd_init(SCI_DEFAULTDIV);
	cmd_loop();

	//we should never get here; if so : die
	die();

}

