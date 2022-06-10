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
#include "ivect_7050.h"

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
void INT_CMT1_CMTI1(void) ISR_attrib;
void INT_CMT1_CMTI1(void) {

	wdt_tog();
	CMT1.CMCNT = 0;
	CMT1.CMCSR.BIT.CMF = 0;	//compare match in lieu of no OCR on ATU
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

#define IVT_ENTRIES 0xA5
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
	WRITEVECT(IVTN_INT_CMT1_CMTI1, &INT_CMT1_CMTI1); // Compare match in lieu of no OCR on ATU

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
	set_vbr((void *) ivt);
	return;
}




/** init free-running main counter, to use for timestamps etc
 * Get current timestamp with MCLK_TS

 * This uses the 32-bit Channel 0. Use prescaler of 1/32 => 625kHz (t=1.6us).
 * This gives a measurable span of ~6872 seconds for intervals. Plenty enough for iso14230
 */

static void init_mclk(void) {
	ATU.TSTR.BIT.STR0 = 0;	//stop while configging
	ATU.PSCR1.BYTE = 0x1F;	//prescaler : 1/32
	ATU0.TIORA.BYTE = 0;	//no input capture
	ATU0.TIERA.BYTE = 0;	//no interrupts
	ATU0.ITVRR.BYTE = 0;	//no ADC intervals
	ATU0.TCNT = 0;
	ATU.TSTR.BIT.STR0 = 1;	//and GO !
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
	ATU.TSTR.WORD = 0;

	init_mclk();
	init_sci();
	init_wdt();
}


/*
 * Assumes the wdt pin setup has already been done,
 * since host was taking care of it just before.
 * This uses Compare Match CNT1 counter 
 */
void init_wdt(void) {
	CMT.CMSTR.BIT.STR1 = 0;	//stop while configging
	CMT1.CMCSR.WORD = 0;

	CMT1.CMCOR = WDT_MAXCNT;
	CMT1.CMCNT = 0;
	INTC.IPRG.BIT._CMT1 = 0x08;	//medium priority for CMT1 (same int)
	CMT1.CMCSR.BIT.CMIE = 1;	//CMT1 compare match int

	CMT1.CMCSR.BIT.CKS = 0x01;		//clksel : 1:32, int_clock/32 => 625kHz(1.6us increments)
	// no prescaler, so 20MHz/32 =625 kHz (1.6us) gives a measurable span of 104 ms
	CMT.CMSTR.BIT.STR1 = 1;		//start only CMCNT1
	return;
}

#define WDT_RSTCSR_SETTING 0x5A4F;	//reset if TCNT overflows

/*
 *
 */
void die(void) {
	set_imask(0x0F);
	set_vbr(0);
	WDT.WRITE.RSTCSR = WDT_RSTCSR_SETTING;
	WDT.WRITE.TCSR = (0xA578 | 0);	// clk div2 for ~ 25us overflow
	while (1) {}
	return;
}

