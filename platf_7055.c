/* platform-specific code that has nothing to do with flashing :
 *
 * - init
 * - master clock
 * - external WDT
 */

/* (c) copyright fenugrec 2016-2022
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
#include "ivect.h"

#include "stypes.h"
#include "platf.h"
#include "wdt.h"

/* init SCI to continue comms on K line */
static void init_sci(void) {
	NPK_SCI.SCR.BYTE &= 0x2F;	//clear TXIE, RXIE, RE
	NPK_SCI.SCR.BIT.TE = 1;	//enable TX
	return;
}


/******* Interrupt stuff */
static void init_wdt(void);

/** WDT toggle interrupt
 *
 * clr timer and flag, easy
 */
void INT_ATU11_IMI1A(void) ISR_attrib;
void INT_ATU11_IMI1A(void) {

	wdt_tog();
	ATU1.TCNTB = 0;
	ATU1.TSRB.BIT.CMF = 0;	//TCNT1B compare match
	return;
}

// ISR handler that saves the previous PC value at the top of RAM, then dies of external WDT.
void die_trace(void) __attribute__ ((noreturn));
void die_trace(void) {
	register u32 tmp asm ("r0");
	asm volatile (
		"mov.l	r0, @-r15\n"
		"stc	sr, r0\n"	//critical part is out of the way; disable ints
		"or	#0xF0, r0\n"
		"ldc	r0, sr\n"

	);
	tmp = (RAM_MAX + 1);
	asm volatile (
		"mov.l	r1, @-r15\n"	//save clobbers for full reg dump !
		"mov	r15, r1\n"
		"add	#0x08, r1\n"
		"mov.l	@r1, r1\n"	//get previous PC val off stack
		"mov.l	r1, @-r0\n"	//save at top-of-ram

						//now, dump all regs
		"mov.l	r15, @-r0\n"
		"mov.l	r14, @-r0\n"
		"mov.l	r13, @-r0\n"
		"mov.l	r12, @-r0\n"
		"mov.l	r11, @-r0\n"
		"mov.l	r10, @-r0\n"
		"mov.l	r9, @-r0\n"
		"mov.l	r8, @-r0\n"
		"mov.l	r7, @-r0\n"
		"mov.l	r6, @-r0\n"
		"mov.l	r5, @-r0\n"
		"mov.l	r4, @-r0\n"
		"mov.l	@r15+, r11\n"	//rescue orig r1 value
		"mov.l	r3, @-r0\n"
		"mov.l	@r15+, r10\n"	//and orig r0
		"mov.l	r2, @-r0\n"
		"mov.l	r11, @-r0\n"
		"mov.l	r10, @-r0\n"
		"0:\n"
		"bra	0b\n"
		"nop\n"

		:
		: "r" (tmp)
		: "r1"
	);
	__builtin_unreachable();
}


/** Runtime-generated IVT (Interrupt / exception vector table).
 * Goal : save ~ 1kB kernel size.
 * Unused entries are set to point to the "dummy" ISR;
 * the vbr reg will be set to point at this table at startup, so this can be anywhere in RAM.
 */

#define IVT_ENTRIES 0x100
#define IVT_DEFAULTENTRY ((u32) &die_trace)
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))

u32 ivt[IVT_ENTRIES];

extern u32 stackinit[];	/* ptr set at linkage */

/** Build an IVT
 */
static void build_ivt(u32 *dest) {
	unsigned i;

	// Fill IVT with dummy entries
	for (i = 0; i < IVT_ENTRIES; i++) {
		dest[i] = IVT_DEFAULTENTRY;
	}

#define WRITEVECT(vectno, ptr) dest[vectno] = (u32) (ptr)

	WRITEVECT(IVTN_POR_SP, stackinit);
	WRITEVECT(IVTN_MR_SP, stackinit);
	WRITEVECT(IVTN_INT_ATU11_IMI1A, &INT_ATU11_IMI1A);

}


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
	UBC.UBCR.BIT.UBID = 1;	//disable UBC
	set_vbr((void *) ivt);
	return;
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



void init_platf(void) {
	build_ivt(ivt);
	init_ints();

	/* stop all timers */
	ATU.TSTR1.BYTE = 0;
	ATU.TSTR2.BYTE = 0;
	ATU.TSTR3.BYTE = 0;

	init_mclk();
	init_sci();
	init_wdt();
}


/*
 * Assumes the wdt pin setup has already been done,
 * since host was taking care of it just before.
 * This uses Channel 1 TCNTB CMF interrupt
 */
static void init_wdt(void) {
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


// internal WDT:  used for 350nm reflash, and for forcing a reset in die(). 
#define WDT_RSTCSR_SETTING 0x5A5F;	//power-on reset if TCNT overflows


/*
 * Some ECUs (VC264) don't seem to reset properly with just the external WDT.
 * But, internal WDT-triggered POR doesn't reset certain periph regs
 * which *may* be causing rare issues (spurious ignition on stopkernel !?).
 * Or were those because I previously wasn't setting vbr=0 ?
 */
void die(void) {
	set_imask(0x0F);
	set_vbr(0);
	trapa(0x3F);
	//unreachable
//	WDT.WRITE.RSTCSR = WDT_RSTCSR_SETTING;
//	WDT.WRITE.TCSR = (0xA578 | 0);	// clk div2 for ~ 12us overflow
	while (1) {}
	return;
}

