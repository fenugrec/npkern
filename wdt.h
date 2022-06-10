#ifndef WDT_H
#define WDT_H

/*
 * (c) fenugrec 2022
 * GPLv3
 */

#include <stdint.h>


extern volatile uint16_t *wdt_dr;	//such as &PL.DR.WORD
extern uint16_t wdt_pin;	//mask for PxDR

/** toggle pin. This is called from a periodic interrupt */
void wdt_tog(void);

#endif
