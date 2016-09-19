/* interrupt handlers;
 * poweron and manual reset are not used
 *
 * Note, this has been gutted a lot and missing some less useful vectors
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


#include "reg_defines/7055_7058_180nm.h"	//io peripheral regs etc
#include "ivect.h"
#include "stypes.h"

//extern void PowerON_Reset_PC(void) {}
//extern void Manual_Reset_PC(void) {}

// 4 Illegal code
//void INT_Illegal_code(void){/* sleep(); */}
// 5 Reserved

// 6 Illegal slot
//void INT_Illegal_slot(void){/* sleep(); */}
// 7 Reserved

// 8 Reserved

// 9 CPU Address error
//void INT_CPU_Address(void){/* sleep(); */}
// 10 DMAC Address error
//void INT_DMAC_Address(void){/* sleep(); */}
// 11 NMI
//void INT_NMI(void){/* sleep(); */}
// 12 User breakpoint trap
//void INT_User_Break(void){/* sleep(); */}
// 13 FPU
//void INT_FPU(void){/* sleep(); */}
// 14 H-UDI
//void INT_HUDI(void){/* sleep(); */}
// 15 Reserved


// replaced with dummies

// 79 Reserved

// 80 ATU01 ITV1/ITV2A/ITV2B
//void INT_ATU01_ITV(void){/* sleep(); */}
// 81 Reserved

// 82 Reserved

// 83 Reserved

// 84 ATU02 ICI0A
//void INT_ATU02_ICI0A(void){/* sleep(); */}
// 85 Reserved

// 86 ATU02 ICI0B
//void INT_ATU02_ICI0B(void){/* sleep(); */}
// 87 Reserved

// 88 ATU03 ICI0C
//void INT_ATU03_ICI0C(void){/* sleep(); */}
// 89 Reserved

// 90 ATU03 ICI0D
//void INT_ATU03_ICI0D(void){/* sleep(); */}
// 91 Reserved

// 92 ATU04 OVI0
//void INT_ATU04_OVI0(void){/* sleep(); */}
// 93 Reserved

// 94 Reserved

// 95 Reserved


// 96 ATU11 IMI1A/CMI1
extern void wdt_tog(void);

void INT_ATU11_IMI1A(void){
	/* WDT toggle interrupt : clr timer and flag, easy . */
	wdt_tog();
	//ATU1.TCNTA = 0;
	ATU1.TCNTB = 0;
	//ATU1.TSRA.BIT.IMFA = 0 ;
	ATU1.TSRB.BIT.CMF = 0;	//TCNT1B compare match
	return;
}

// 97 ATU11 IMI1B
//void INT_ATU11_IMI1B(void){/* sleep(); */}
// 98 ATU11 IMI1C
//void INT_ATU11_IMI1C(void){/* sleep(); */}
// 99 ATU11 IMI1D
//void INT_ATU11_IMI1D(void){/* sleep(); */}
// 100 ATU12 IMI1E
//void INT_ATU12_IMI1E(void){/* sleep(); */}
// 101 ATU12 IMI1F
//void INT_ATU12_IMI1F(void){/* sleep(); */}
// 102 ATU12 IMI1G
//void INT_ATU12_IMI1G(void){/* sleep(); */}
// 103 ATU12 IMI1H
//void INT_ATU12_IMI1H(void){/* sleep(); */}
// 104 ATU13 OVI1A/OVI1B
//void INT_ATU13_OVI1(void){/* sleep(); */}
// 105 Reserved

// 106 Reserved


//replaced with dummies
// 173 Reserved

// 174 ATU92 CMI9F
//void INT_ATU92_CMI9F(void){/* sleep(); */}
// 175 Reserved

// 176 ATU101 CMI10A
//void INT_ATU101_CMI10A(void){/* sleep(); */}
// 177 Reserved

// 178 ATU101 CMI10B
//void INT_ATU101_CMI10B(void){/* sleep(); */}
// 179 Reserved

// 180 ATU102 ICI10A/CMI10G
//void INT_ATU102_ICI10A(void){/* sleep(); */}
// 181 Reserved

// 182 Reserved

// 183 Reserved

// 184 ATU11 IMI11A
//void INT_ATU11_IMI11A(void){/* sleep(); */}
// 185 Reserved

// 186 ATU11 IMI11B
//void INT_ATU11_IMI11B(void){/* sleep(); */}
// 187 ATU11 OVI11
//void INT_ATU11_OVI11(void){/* sleep(); */}
// 188 CMT0 CMTI0
//void INT_CMT0_CMTI0(void){/* sleep(); */}
// 189 Reserved

// 190 A/D0 ADI0
//void INT_AD0_ADI0(void){/* sleep(); */}
// 191 Reserved

// 192 CMT1 CMTI1
//void INT_CMT1_CMTI1(void){/* sleep(); */}
// 193 Reserved

// 194 A/D1 ADI1
//void INT_AD1_ADI1(void){/* sleep(); */}
// 195 Reserved

// 196 A/D2 ADI2
//void INT_AD2_ADI2(void){/* sleep(); */}
// 197 Reserved

// 198 Reserved

// 199 Reserved

// 200 SCI0 ERI0
//void INT_SCI0_ERI0(void){/* sleep(); */}
// 201 SCI0 RXI0
//void INT_SCI0_RXI0(void){/* sleep(); */}
// 202 SCI0 TXI0
//void INT_SCI0_TXI0(void){/* sleep(); */}
// 203 SCI0 TEI0
//void INT_SCI0_TEI0(void){/* sleep(); */}

// 204 SCI1 ERI1
/* these are ignored */
//void INT_SCI1_ERI1(void){/* sleep(); */}


// 205 SCI1 RXI1

// 206 SCI1 TXI1
//void INT_SCI1_TXI1(void){/* sleep(); */}
// 207 SCI1 TEI1
//void INT_SCI1_TEI1(void){/* sleep(); */}


// 225 Reserved

// 226 Reserved

// 227 Reserved

// 228 HCAN1 ERS1
//void INT_HCAN1_ERS1(void){/* sleep(); */}
// 229 HCAN1 OVR1
//void INT_HCAN1_OVR1(void){/* sleep(); */}
// 230 HCAN1 RM1
//void INT_HCAN1_RM1(void){/* sleep(); */}
// 231 HCAN1 SLE1
//void INT_HCAN1_SLE1(void){/* sleep(); */}
// 232 Reserved

// 233 Reserved


// 255 Reserved


