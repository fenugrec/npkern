/* Interrupt / exception vector table. Unused entries are set to point to the "dummy" ISR;
 * the vbr reg will be set to point at this table at startup, so this can be anywhere in RAM.
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



#include "ivect.h"
#include "stypes.h"

/* stuff filled at linkage : */
extern u32 stackinit[];

// Dummy handler
void dummy(void){
	return;
	}

#pragma weak PowerON_Reset_PC = dummy
#pragma weak Manual_Reset_PC = dummy
#pragma weak INT_Illegal_code = dummy
#pragma weak INT_Illegal_slot = dummy
#pragma weak INT_CPU_Address = dummy
#pragma weak INT_DMAC_Address = dummy
#pragma weak INT_NMI = dummy
#pragma weak INT_User_Break = dummy
#pragma weak INT_FPU = dummy
#pragma weak INT_HUDI = dummy
#pragma weak INT_TRAPA32 = dummy
#pragma weak INT_TRAPA33 = dummy
#pragma weak INT_TRAPA34 = dummy
#pragma weak INT_TRAPA35 = dummy
#pragma weak INT_TRAPA36 = dummy
#pragma weak INT_TRAPA37 = dummy
#pragma weak INT_TRAPA38 = dummy
#pragma weak INT_TRAPA39 = dummy
#pragma weak INT_TRAPA40 = dummy
#pragma weak INT_TRAPA41 = dummy
#pragma weak INT_TRAPA42 = dummy
#pragma weak INT_TRAPA43 = dummy
#pragma weak INT_TRAPA44 = dummy
#pragma weak INT_TRAPA45 = dummy
#pragma weak INT_TRAPA46 = dummy
#pragma weak INT_TRAPA47 = dummy
#pragma weak INT_TRAPA48 = dummy
#pragma weak INT_TRAPA49 = dummy
#pragma weak INT_TRAPA50 = dummy
#pragma weak INT_TRAPA51 = dummy
#pragma weak INT_TRAPA52 = dummy
#pragma weak INT_TRAPA53 = dummy
#pragma weak INT_TRAPA54 = dummy
#pragma weak INT_TRAPA55 = dummy
#pragma weak INT_TRAPA56 = dummy
#pragma weak INT_TRAPA57 = dummy
#pragma weak INT_TRAPA58 = dummy
#pragma weak INT_TRAPA59 = dummy
#pragma weak INT_TRAPA60 = dummy
#pragma weak INT_TRAPA61 = dummy
#pragma weak INT_TRAPA62 = dummy
#pragma weak INT_TRAPA63 = dummy
#pragma weak INT_IRQ0 = dummy
#pragma weak INT_IRQ1 = dummy
#pragma weak INT_IRQ2 = dummy
#pragma weak INT_IRQ3 = dummy
#pragma weak INT_IRQ4 = dummy
#pragma weak INT_IRQ5 = dummy
#pragma weak INT_IRQ6 = dummy
#pragma weak INT_IRQ7 = dummy
#pragma weak INT_DMAC0_DEI0 = dummy
#pragma weak INT_DMAC1_DEI1 = dummy
#pragma weak INT_DMAC2_DEI2 = dummy
#pragma weak INT_DMAC3_DEI3 = dummy
#pragma weak INT_ATU01_ITV = dummy
#pragma weak INT_ATU02_ICI0A = dummy
#pragma weak INT_ATU02_ICI0B = dummy
#pragma weak INT_ATU03_ICI0C = dummy
#pragma weak INT_ATU03_ICI0D = dummy
#pragma weak INT_ATU04_OVI0 = dummy
#pragma weak INT_ATU11_IMI1A = dummy
#pragma weak INT_ATU11_IMI1B = dummy
#pragma weak INT_ATU11_IMI1C = dummy
#pragma weak INT_ATU11_IMI1D = dummy
#pragma weak INT_ATU12_IMI1E = dummy
#pragma weak INT_ATU12_IMI1F = dummy
#pragma weak INT_ATU12_IMI1G = dummy
#pragma weak INT_ATU12_IMI1H = dummy
#pragma weak INT_ATU13_OVI1 = dummy
#pragma weak INT_ATU21_IMI2A = dummy
#pragma weak INT_ATU21_IMI2B = dummy
#pragma weak INT_ATU21_IMI2C = dummy
#pragma weak INT_ATU21_IMI2D = dummy
#pragma weak INT_ATU22_IMI2E = dummy
#pragma weak INT_ATU22_IMI2F = dummy
#pragma weak INT_ATU22_IMI2G = dummy
#pragma weak INT_ATU22_IMI2H = dummy
#pragma weak INT_ATU23_OVI2 = dummy
#pragma weak INT_ATU31_IMI3A = dummy
#pragma weak INT_ATU31_IMI3B = dummy
#pragma weak INT_ATU31_IMI3C = dummy
#pragma weak INT_ATU31_IMI3D = dummy
#pragma weak INT_ATU32_OVI3 = dummy
#pragma weak INT_ATU41_IMI4A = dummy
#pragma weak INT_ATU41_IMI4B = dummy
#pragma weak INT_ATU41_IMI4C = dummy
#pragma weak INT_ATU41_IMI4D = dummy
#pragma weak INT_ATU42_OVI4 = dummy
#pragma weak INT_ATU51_IMI5A = dummy
#pragma weak INT_ATU51_IMI5B = dummy
#pragma weak INT_ATU51_IMI5C = dummy
#pragma weak INT_ATU51_IMI5D = dummy
#pragma weak INT_ATU52_OVI5 = dummy
#pragma weak INT_ATU6_CMI6A = dummy
#pragma weak INT_ATU6_CMI6B = dummy
#pragma weak INT_ATU6_CMI6C = dummy
#pragma weak INT_ATU6_CMI6D = dummy
#pragma weak INT_ATU7_CMI7A = dummy
#pragma weak INT_ATU7_CMI7B = dummy
#pragma weak INT_ATU7_CMI7C = dummy
#pragma weak INT_ATU7_CMI7D = dummy
#pragma weak INT_ATU81_OSI8A = dummy
#pragma weak INT_ATU81_OSI8B = dummy
#pragma weak INT_ATU81_OSI8C = dummy
#pragma weak INT_ATU81_OSI8D = dummy
#pragma weak INT_ATU82_OSI8E = dummy
#pragma weak INT_ATU82_OSI8F = dummy
#pragma weak INT_ATU82_OSI8G = dummy
#pragma weak INT_ATU82_OSI8H = dummy
#pragma weak INT_ATU83_OSI8I = dummy
#pragma weak INT_ATU83_OSI8J = dummy
#pragma weak INT_ATU83_OSI8K = dummy
#pragma weak INT_ATU83_OSI8L = dummy
#pragma weak INT_ATU84_OSI8M = dummy
#pragma weak INT_ATU84_OSI8N = dummy
#pragma weak INT_ATU84_OSI8O = dummy
#pragma weak INT_ATU84_OSI8P = dummy
#pragma weak INT_ATU91_CMI9A = dummy
#pragma weak INT_ATU91_CMI9B = dummy
#pragma weak INT_ATU91_CMI9C = dummy
#pragma weak INT_ATU91_CMI9D = dummy
#pragma weak INT_ATU92_CMI9E = dummy
#pragma weak INT_ATU92_CMI9F = dummy
#pragma weak INT_ATU101_CMI10A = dummy
#pragma weak INT_ATU101_CMI10B = dummy
#pragma weak INT_ATU102_ICI10A = dummy
#pragma weak INT_ATU11_IMI11A = dummy
#pragma weak INT_ATU11_IMI11B = dummy
#pragma weak INT_ATU11_OVI11 = dummy
#pragma weak INT_CMT0_CMTI0 = dummy
#pragma weak INT_AD0_ADI0 = dummy
#pragma weak INT_CMT1_CMTI1 = dummy
#pragma weak INT_AD1_ADI1 = dummy
#pragma weak INT_AD2_ADI2 = dummy
#pragma weak INT_SCI0_ERI0 = dummy
#pragma weak INT_SCI0_RXI0 = dummy
#pragma weak INT_SCI0_TXI0 = dummy
#pragma weak INT_SCI0_TEI0 = dummy
#pragma weak INT_SCI1_ERI1 = dummy
#pragma weak INT_SCI1_RXI1 = dummy
#pragma weak INT_SCI1_TXI1 = dummy
#pragma weak INT_SCI1_TEI1 = dummy
#pragma weak INT_SCI2_ERI2 = dummy
#pragma weak INT_SCI2_RXI2 = dummy
#pragma weak INT_SCI2_TXI2 = dummy
#pragma weak INT_SCI2_TEI2 = dummy
#pragma weak INT_SCI3_ERI3 = dummy
#pragma weak INT_SCI3_RXI3 = dummy
#pragma weak INT_SCI3_TXI3 = dummy
#pragma weak INT_SCI3_TEI3 = dummy
#pragma weak INT_SCI4_ERI4 = dummy
#pragma weak INT_SCI4_RXI4 = dummy
#pragma weak INT_SCI4_TXI4 = dummy
#pragma weak INT_SCI4_TEI4 = dummy
#pragma weak INT_HCAN0_ERS0 = dummy
#pragma weak INT_HCAN0_OVR0 = dummy
#pragma weak INT_HCAN0_RM0 = dummy
#pragma weak INT_HCAN0_SLE0 = dummy
#pragma weak INT_WDT_ITI = dummy
#pragma weak INT_HCAN1_ERS1 = dummy
#pragma weak INT_HCAN1_OVR1 = dummy
#pragma weak INT_HCAN1_RM1 = dummy
#pragma weak INT_HCAN1_SLE1 = dummy

const void * ivt[] = {
//;0 Power On Reset PC
	(void*)	PowerON_Reset_PC,																												
// 1 Power On Reset SP
	stackinit,
//;2 Manual Reset PC
	(void*)	Manual_Reset_PC,																												 
// 3 Manual Reset SP
	stackinit,
// 4 Illegal code
	(void*) INT_Illegal_code,
// 5 Reserved
	(void*) dummy,
// 6 Illegal slot
	(void*) INT_Illegal_slot,
// 7 Reserved
	(void*) dummy,
// 8 Reserved
	(void*) dummy,
// 9 CPU Address error
	(void*) INT_CPU_Address,
// 10 DMAC Address error
	(void*) INT_DMAC_Address,
// 11 NMI
	(void*) INT_NMI,
// 12 User breakpoint trap
	(void*) INT_User_Break,
// 13 FPU
	(void*) INT_FPU,
// 14 H-UDI
	(void*) INT_HUDI,
// 15 Reserved
	(void*) dummy,
// 16 Reserved
	(void*) dummy,
// 17 Reserved
	(void*) dummy,
// 18 Reserved
	(void*) dummy,
// 19 Reserved
	(void*) dummy,
// 20 Reserved
	(void*) dummy,
// 21 Reserved
	(void*) dummy,
// 22 Reserved
	(void*) dummy,
// 23 Reserved
	(void*) dummy,
// 24 Reserved
	(void*) dummy,
// 25 Reserved
	(void*) dummy,
// 26 Reserved
	(void*) dummy,
// 27 Reserved
	(void*) dummy,
// 28 Reserved
	(void*) dummy,
// 29 Reserved
	(void*) dummy,
// 30 Reserved
	(void*) dummy,
// 31 Reserved
	(void*) dummy,
// 32 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA32
// 33 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA33
// 34 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA34
// 35 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA35
// 36 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA36
// 37 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA37
// 38 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA38
// 39 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA39
// 40 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA40
// 41 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA41
// 42 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA42
// 43 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA43
// 44 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA44
// 45 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA45
// 46 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA46
// 47 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA47
// 48 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA48
// 49 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA49
// 50 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA50
// 51 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA51
// 52 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA52
// 53 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA53
// 54 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA54
// 55 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA55
// 56 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA56
// 57 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA57
// 58 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA58
// 59 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA59
// 60 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA60
// 61 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA61
// 62 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA62
// 63 TRAPA (User Vecter)
	(void*) dummy,	//INT_TRAPA63
// 64 Interrupt IRQ0
	(void*) dummy,	//INT_IRQ0
// 65 Interrupt IRQ1
	(void*) dummy,	//INT_IRQ1
// 66 Interrupt IRQ2
	(void*) dummy,	//INT_IRQ2
// 67 Interrupt IRQ3
	(void*) dummy,	//INT_IRQ3
// 68 Interrupt IRQ4
	(void*) dummy,	//INT_IRQ4
// 69 Interrupt IRQ5
	(void*) dummy,	//INT_IRQ5
// 70 Interrupt IRQ6
	(void*) dummy,	//INT_IRQ6
// 71 Interrupt IRQ7
	(void*) dummy,	//INT_IRQ7
// 72 DMAC0 DEI0
	(void*) dummy,	//INT_DMAC0_DEI0
// 73 Reserved
	(void*) dummy,	//dummy
// 74 DMAC1 DEI1
	(void*) dummy,	//INT_DMAC1_DEI1
// 75 Reserved
	(void*) dummy,	//dummy
// 76 DMAC2 DEI2
	(void*) dummy,	//INT_DMAC2_DEI2
// 77 Reserved
	(void*) dummy,	//dummy
// 78 DMAC3 DEI3
	(void*) dummy,	//INT_DMAC3_DEI3
// 79 Reserved
	(void*) dummy,
// 80 ATU01 ITV1/ITV2A/ITV2B
	(void*) INT_ATU01_ITV,
// 81 Reserved
	(void*) dummy,
// 82 Reserved
	(void*) dummy,
// 83 Reserved
	(void*) dummy,
// 84 ATU02 ICI0A
	(void*) INT_ATU02_ICI0A,
// 85 Reserved
	(void*) dummy,
// 86 ATU02 ICI0B
	(void*) INT_ATU02_ICI0B,
// 87 Reserved
	(void*) dummy,
// 88 ATU03 ICI0C
	(void*) INT_ATU03_ICI0C,
// 89 Reserved
	(void*) dummy,
// 90 ATU03 ICI0D
	(void*) INT_ATU03_ICI0D,
// 91 Reserved
	(void*) dummy,
// 92 ATU04 OVI0
	(void*) INT_ATU04_OVI0,
// 93 Reserved
	(void*) dummy,
// 94 Reserved
	(void*) dummy,
// 95 Reserved
	(void*) dummy,
// 96 ATU11 IMI1A/CMI1
	(void*) INT_ATU11_IMI1A,
// 97 ATU11 IMI1B
	(void*) INT_ATU11_IMI1B,
// 98 ATU11 IMI1C
	(void*) INT_ATU11_IMI1C,
// 99 ATU11 IMI1D
	(void*) INT_ATU11_IMI1D,
// 100 ATU12 IMI1E
	(void*) INT_ATU12_IMI1E,
// 101 ATU12 IMI1F
	(void*) INT_ATU12_IMI1F,
// 102 ATU12 IMI1G
	(void*) INT_ATU12_IMI1G,
// 103 ATU12 IMI1H
	(void*) INT_ATU12_IMI1H,
// 104 ATU13 OVI1A/OVI1B
	(void*) INT_ATU13_OVI1,
// 105 Reserved
	(void*) dummy,
// 106 Reserved
	(void*) dummy,
// 107 Reserved
	(void*) dummy,
// 108 ATU21 IMI2A/CMI2A
	(void*) dummy,	//INT_ATU21_IMI2A
// 109 ATU21 IMI2B/CMI2B
	(void*) dummy,	//INT_ATU21_IMI2B
// 110 ATU21 IMI2C/CMI2C
	(void*) dummy,	//INT_ATU21_IMI2C
// 111 ATU21 IMI2D/CMI2D
	(void*) dummy,	//INT_ATU21_IMI2D
// 112 ATU22 IMI2E/CMI2E
	(void*) dummy,	//INT_ATU22_IMI2E
// 113 ATU22 IMI2F/CMI2F
	(void*) dummy,	//INT_ATU22_IMI2F
// 114 ATU22 IMI2G/CMI2G
	(void*) dummy,	//INT_ATU22_IMI2G
// 115 ATU22 IMI2H/CMI2H
	(void*) dummy,	//INT_ATU22_IMI2H
// 116 ATU23 OVI2A/OVI2B
	(void*) dummy,	//INT_ATU23_OVI2
// 117 Reserved
	(void*) dummy,
// 118 Reserved
	(void*) dummy,
// 119 Reserved
	(void*) dummy,
// 120 ATU31 IMI3A
	(void*) dummy,	//INT_ATU31_IMI3A
// 121 ATU31 IMI3B
	(void*) dummy,	//INT_ATU31_IMI3B
// 122 ATU31 IMI3C
	(void*) dummy,	//INT_ATU31_IMI3C
// 123 ATU31 IMI3D
	(void*) dummy,	//INT_ATU31_IMI3D
// 124 ATU32 OVI3
	(void*) dummy,	//INT_ATU32_OVI3
// 125 Reserved
	(void*) dummy,
// 126 Reserved
	(void*) dummy,
// 127 Reserved
	(void*) dummy,
// 128 ATU41 IMI4A
	(void*) dummy,	//INT_ATU41_IMI4A
// 129 ATU41 IMI4B
	(void*) dummy,	//INT_ATU41_IMI4B
// 130 ATU41 IMI4C
	(void*) dummy,	//INT_ATU41_IMI4C
// 131 ATU41 IMI4D
	(void*) dummy,	//INT_ATU41_IMI4D
// 132 ATU42 OVI4
	(void*) dummy,	//INT_ATU42_OVI4
// 133 Reserved
	(void*) dummy,
// 134 Reserved
	(void*) dummy,
// 135 Reserved
	(void*) dummy,
// 136 ATU51 IMI5A
	(void*) dummy,	//INT_ATU51_IMI5A
// 137 ATU51 IMI5B
	(void*) dummy,	//INT_ATU51_IMI5B
// 138 ATU51 IMI5C
	(void*) dummy,	//INT_ATU51_IMI5C
// 139 ATU51 IMI5D
	(void*) dummy,	//INT_ATU51_IMI5D
// 140 ATU52 OVI5
	(void*) dummy,	//INT_ATU52_OVI5
// 141 Reserved
	(void*) dummy,
// 142 Reserved
	(void*) dummy,
// 143 Reserved
	(void*) dummy,
// 144 ATU6 CMI6A
	(void*) dummy,	//INT_ATU6_CMI6A
// 145 ATU6 CMI6B
	(void*) dummy,	//INT_ATU6_CMI6B
// 146 ATU6 CMI6C
	(void*) dummy,	//INT_ATU6_CMI6C
// 147 ATU6 CMI6D
	(void*) dummy,	//INT_ATU6_CMI6D
// 148 ATU7 CMI7A
	(void*) dummy,	//INT_ATU7_CMI7A
// 149 ATU7 CMI7B
	(void*) dummy,	//INT_ATU7_CMI7B
// 150 ATU7 CMI7C
	(void*) dummy,	//INT_ATU7_CMI7C
// 151 ATU7 CMI7D
	(void*) dummy,	//INT_ATU7_CMI7D
// 152 ATU81 OSI8A
	(void*) dummy,	//INT_ATU81_OSI8A
// 153 ATU81 OSI8B
	(void*) dummy,	//INT_ATU81_OSI8B
// 154 ATU81 OSI8C
	(void*) dummy,	//INT_ATU81_OSI8C
// 155 ATU81 OSI8D
	(void*) dummy,	//INT_ATU81_OSI8D
// 156 ATU82 OSI8E
	(void*) dummy,	//INT_ATU82_OSI8E
// 157 ATU82 OSI8F
	(void*) dummy,	//INT_ATU82_OSI8F
// 158 ATU82 OSI8G
	(void*) dummy,	//INT_ATU82_OSI8G
// 159 ATU82 OSI8H
	(void*) dummy,	//INT_ATU82_OSI8H
// 160 ATU83 OSI8I
	(void*) dummy,	//INT_ATU83_OSI8I
// 161 ATU83 OSI8J
	(void*) dummy,	//INT_ATU83_OSI8J
// 162 ATU83 OSI8K
	(void*) dummy,	//INT_ATU83_OSI8K
// 163 ATU83 OSI8L
	(void*) dummy,	//INT_ATU83_OSI8L
// 164 ATU84 OSI8M
	(void*) dummy,	//INT_ATU84_OSI8M
// 165 ATU84 OSI8N
	(void*) dummy,	//INT_ATU84_OSI8N
// 166 ATU84 OSI8O
	(void*) dummy,	//INT_ATU84_OSI8O
// 167 ATU84 OSI8P
	(void*) dummy,	//INT_ATU84_OSI8P
// 168 ATU91 CMI9A
	(void*) dummy,	//INT_ATU91_CMI9A
// 169 ATU91 CMI9B
	(void*) dummy,	//INT_ATU91_CMI9B
// 170 ATU91 CMI9C
	(void*) dummy,	//INT_ATU91_CMI9C
// 171 ATU91 CMI9D
	(void*) dummy,	//INT_ATU91_CMI9D
// 172 ATU92 CMI9E
	(void*) dummy,	//INT_ATU92_CMI9E
// 173 Reserved
	(void*) dummy,
// 174 ATU92 CMI9F
	(void*) INT_ATU92_CMI9F,
// 175 Reserved
	(void*) dummy,
// 176 ATU101 CMI10A
	(void*) INT_ATU101_CMI10A,
// 177 Reserved
	(void*) dummy,
// 178 ATU101 CMI10B
	(void*) INT_ATU101_CMI10B,
// 179 Reserved
	(void*) dummy,
// 180 ATU102 ICI10A/CMI10G
	(void*) INT_ATU102_ICI10A,
// 181 Reserved
	(void*) dummy,
// 182 Reserved
	(void*) dummy,
// 183 Reserved
	(void*) dummy,
// 184 ATU11 IMI11A
	(void*) INT_ATU11_IMI11A,
// 185 Reserved
	(void*) dummy,
// 186 ATU11 IMI11B
	(void*) INT_ATU11_IMI11B,
// 187 ATU11 OVI11
	(void*) INT_ATU11_OVI11,
// 188 CMT0 CMTI0
	(void*) INT_CMT0_CMTI0,
// 189 Reserved
	(void*) dummy,
// 190 A/D0 ADI0
	(void*) INT_AD0_ADI0,
// 191 Reserved
	(void*) dummy,
// 192 CMT1 CMTI1
	(void*) INT_CMT1_CMTI1,
// 193 Reserved
	(void*) dummy,
// 194 A/D1 ADI1
	(void*) INT_AD1_ADI1,
// 195 Reserved
	(void*) dummy,
// 196 A/D2 ADI2
	(void*) INT_AD2_ADI2,
// 197 Reserved
	(void*) dummy,
// 198 Reserved
	(void*) dummy,
// 199 Reserved
	(void*) dummy,
// 200 SCI0 ERI0
	(void*) INT_SCI0_ERI0,
// 201 SCI0 RXI0
	(void*) INT_SCI0_RXI0,
// 202 SCI0 TXI0
	(void*) INT_SCI0_TXI0,
// 203 SCI0 TEI0
	(void*) INT_SCI0_TEI0,
// 204 SCI1 ERI1
	(void*) INT_SCI1_ERI1,
// 205 SCI1 RXI1
	(void*) INT_SCI1_RXI1,
// 206 SCI1 TXI1
	(void*) INT_SCI1_TXI1,
// 207 SCI1 TEI1
	(void*) INT_SCI1_TEI1,
// 208 SCI2 ERI2
	(void*) dummy,	//INT_SCI2_ERI2
// 209 SCI2 RXI2
	(void*) dummy,	//INT_SCI2_RXI2
// 210 SCI2 TXI2
	(void*) dummy,	//INT_SCI2_TXI2
// 211 SCI2 TEI2
	(void*) dummy,	//INT_SCI2_TEI2
// 212 SCI3 ERI3
	(void*) dummy,	//INT_SCI3_ERI3
// 213 SCI3 RXI3
	(void*) dummy,	//INT_SCI3_RXI3
// 214 SCI3 TXI3
	(void*) dummy,	//INT_SCI3_TXI3
// 215 SCI3 TEI3
	(void*) dummy,	//INT_SCI3_TEI3
// 216 SCI4 ERI4
	(void*) dummy,	//INT_SCI4_ERI4
// 217 SCI4 RXI4
	(void*) dummy,	//INT_SCI4_RXI4
// 218 SCI4 TXI4
	(void*) dummy,	//INT_SCI4_TXI4
// 219 SCI4 TEI4
	(void*) dummy,	//INT_SCI4_TEI4
// 220 HCAN0 ERS0
	(void*) dummy,	//INT_HCAN0_ERS0
// 221 HCAN0 OVR0
	(void*) dummy,	//INT_HCAN0_OVR0
// 222 HCAN0 RM0
	(void*) dummy,	//INT_HCAN0_RM0
// 223 HCAN0 SLE0
	(void*) dummy,	//INT_HCAN0_SLE0
// 224 WDT ITI
	(void*) dummy,	//INT_WDT_ITI
// 225 Reserved
	(void*) dummy,
// 226 Reserved
	(void*) dummy,
// 227 Reserved
	(void*) dummy,
// 228 HCAN1 ERS1
	(void*) INT_HCAN1_ERS1,
// 229 HCAN1 OVR1
	(void*) INT_HCAN1_OVR1,
// 230 HCAN1 RM1
	(void*) INT_HCAN1_RM1,
// 231 HCAN1 SLE1
	(void*) INT_HCAN1_SLE1,
// 232 Reserved
	(void*) dummy,
// 233 Reserved
	(void*) dummy,
// 234 Reserved
	(void*) dummy,
// 235 Reserved
	(void*) dummy,
// 236 Reserved
	(void*) dummy,
// 237 Reserved
	(void*) dummy,
// 238 Reserved
	(void*) dummy,
// 239 Reserved
	(void*) dummy,
// 240 Reserved
	(void*) dummy,
// 241 Reserved
	(void*) dummy,
// 242 Reserved
	(void*) dummy,
// 243 Reserved
	(void*) dummy,
// 244 Reserved
	(void*) dummy,
// 245 Reserved
	(void*) dummy,
// 246 Reserved
	(void*) dummy,
// 247 Reserved
	(void*) dummy,
// 248 Reserved
	(void*) dummy,
// 249 Reserved
	(void*) dummy,
// 250 Reserved
	(void*) dummy,
// 251 Reserved
	(void*) dummy,
// 252 Reserved
	(void*) dummy,
// 253 Reserved
	(void*) dummy,
// 254 Reserved
	(void*) dummy,
// 255 Reserved
	(void*) dummy
};
