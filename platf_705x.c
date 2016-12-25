/* platform-specific code that has nothing to do with flashing :
 *
 * - init
 * - master clock
 * - external WDT
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
#include "ivect.h"

#include "stypes.h"
#include "platf.h"

/* init SCI1 to continue comms on K line */
static void init_sci(void) {
	SCI1.SCR.BYTE &= 0x2F;	//clear TXIE, RXIE, RE
	SCI1.SCR.BIT.TE = 1;	//enable TX
	return;
}

void die(void);

// Dummy ISR handler
void dummy(void){
	die();
	return;
}


/** Runtime-generated IVT (Interrupt / exception vector table).
 * Goal : save ~ 1kB kernel size.
 * Unused entries are set to point to the "dummy" ISR;
 * the vbr reg will be set to point at this table at startup, so this can be anywhere in RAM.
 */

#define IVT_ENTRIES 0x100
#define IVT_DEFAULTENTRY ((u32) &dummy)
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))

extern u32 stackinit[];	/* ptr set at linkage */
u32 ivt[IVT_ENTRIES];


/** Builds an IVT
 */
static void build_ivt(u32 *dest) {
	unsigned i;

	// Fill IVT with dummy entries
	for (i = 0; i < IVT_ENTRIES; i++) {
		dest[i] = IVT_DEFAULTENTRY;
	}

#define WRITEVECT(vectno, ptr) dest[vectno] = (u32) (ptr)

	WRITEVECT(1, stackinit);
	WRITEVECT(3, stackinit);
	WRITEVECT(96, &INT_ATU11_IMI1A);

}




/** init free-running main counter, to use for timestamps etc
 * Get current timestamp with MCLK_TS

 * This uses the 32-bit Channel 0. It usees the same prescaler (#1)
 * as Channel 1, so 1/32 => 625kHz (t=1.6us). This gives a measurable
 * span of ~6872 seconds for intervals. Plenty enough for iso14230
 */

static void init_mclk(void) {
	ATU.TSTR1.BIT.STR0 = 0;	//stop while configging
	ATU0.TIOR.BYTE = 0;	//no input capture
	ATU0.TIER.WORD = 0;	//no interrupts
	ATU0.ITVRR1.BYTE = 0;
	ATU0.ITVRR2A.BYTE = 0;
	ATU0.ITVRR2B.BYTE = 0;	//no ADC intervals
	ATU0.TCNT = 0;
	ATU.TSTR1.BIT.STR0 = 1;	//and GO !
	return;
}

#if 0
uint32_t get_mclk_ts(void) {
	return ATU0.TCNT;
}
#endif


/** init interrupts : set all prios to 0, set vbr */
static void init_ints(void) {
	INTC.IPRA.WORD = 0;
	INTC.IPRB.WORD = 0;
	INTC.IPRC.WORD = 0;
	INTC.IPRD.WORD = 0;
	INTC.IPRE.WORD = 0;
	INTC.IPRF.WORD = 0;
	INTC.IPRG.WORD = 0;
	INTC.IPRH.WORD = 0;
	INTC.IPRI.WORD = 0;
	INTC.IPRJ.WORD = 0;
	INTC.IPRK.WORD = 0;
	INTC.IPRL.WORD = 0;
	set_vbr((void *) ivt);
	return;
}



void init_platf(void) {
	build_ivt(ivt);
	init_ints();

	/* stop all timers */
	ATU.TSTR1.BYTE = 0;
	ATU.TSTR2.BYTE = 0;
	ATU.TSTR3.BYTE = 0;

	init_mclk();
	init_sci();

}


/*
 * Assumes the wdt pin setup has already been done,
 * since host was taking care of it just before.
 * This uses Channel 1 TCNTB CMF interrupt
 */
void init_wdt(void) {
	ATU.TSTR1.BIT.STR12B = 0;	//stop while configging
	ATU1.TIORA.BYTE = 0;
	ATU1.TIORB.BYTE = 0;
	ATU1.TIORC.BYTE = 0;
	ATU1.TIORD.BYTE = 0;
	//ATU1.TSRA.WORD = 0;
	ATU1.TSRB.WORD = 0;

	ATU1.OCR = WDT_MAXCNT;
	ATU1.TCNTB = 0;
	INTC.IPRD.BIT._ATU11 = 0x08;	//medium priority for ATU11_IMI1A / CM1 (same int)
	ATU1.TIERA.WORD = 0;	//disable other IMI1{A,B,C,D} !!
	ATU1.TIERB.BIT.CME = 1;	//TCNTB compare match int

	ATU.PSCR1.BYTE = 0x1F;	//prescaler : 1/32
	ATU1.TCRB.BYTE = 0;		//clksel : 1:1 , from int_clock/2 => 625kHz(1.6us increments)
	ATU.TSTR1.BIT.STR12B = 1;		//start only TCNT1B
	return;
}


/* spinloop for X millisecs */
#if 0	//unused atm
void waitms(unsigned ms) {
	uint32_t t0, t1;
	t0 = ATU0.TCNT;
	t1 = t0 + MCLK_GETTS(ms);
	while (ATU0.TCNT < t1) {}
	return;
}
#endif
