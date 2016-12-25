#ifndef _IVECT_H
#define _IVECT_H
/***********************************************************************/
/* 7055/7058 vectors; these are almost identical accross all 7055/7058 variants.
 * Only differences are maybe some of the ATU* vector names
 *
 */
/***********************************************************************/

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




#define ISR __attribute__ ((interrupt_handler))

extern void dummy(void) ISR;
//#define WEAKALIAS __attribute__ ((weak, alias ("dummy")))
#define WEAKALIAS

//;<<VECTOR DATA START (POWER ON RESET)>>
//;0 Power On Reset PC
extern void WEAKALIAS PowerON_Reset_PC(void) ISR;

//;<<VECTOR DATA END (POWER ON RESET)>>
// 1 Power On Reset SP

//;<<VECTOR DATA START (MANUAL RESET)>>
//;2 Manual Reset PC
extern void WEAKALIAS Manual_Reset_PC(void) ISR;

//;<<VECTOR DATA END (MANUAL RESET)>>
// 3 Manual Reset SP

// 4 Illegal code
extern void WEAKALIAS INT_Illegal_code(void) ISR;

// 5 Reserved

// 6 Illegal slot
extern void WEAKALIAS INT_Illegal_slot(void) ISR;

// 7 Reserved

// 8 Reserved

// 9 CPU Address error
extern void WEAKALIAS INT_CPU_Address(void) ISR;

// 10 DMAC Address error
extern void WEAKALIAS INT_DMAC_Address(void) ISR;

// 11 NMI
extern void WEAKALIAS INT_NMI(void) ISR;

// 12 User breakpoint trap
extern void WEAKALIAS INT_User_Break(void) ISR;

// 13 FPU
extern void WEAKALIAS INT_FPU(void) ISR;

// 14 H-UDI
extern void WEAKALIAS INT_HUDI(void) ISR;

// 15 Reserved

// 16 Reserved

// 17 Reserved

// 18 Reserved

// 19 Reserved

// 20 Reserved

// 21 Reserved

// 22 Reserved

// 23 Reserved

// 24 Reserved

// 25 Reserved

// 26 Reserved

// 27 Reserved

// 28 Reserved

// 29 Reserved

// 30 Reserved

// 31 Reserved

// 32 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA32(void) ISR;

// 33 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA33(void) ISR;

// 34 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA34(void) ISR;

// 35 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA35(void) ISR;

// 36 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA36(void) ISR;

// 37 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA37(void) ISR;

// 38 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA38(void) ISR;

// 39 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA39(void) ISR;

// 40 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA40(void) ISR;

// 41 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA41(void) ISR;

// 42 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA42(void) ISR;

// 43 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA43(void) ISR;

// 44 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA44(void) ISR;

// 45 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA45(void) ISR;

// 46 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA46(void) ISR;

// 47 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA47(void) ISR;

// 48 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA48(void) ISR;

// 49 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA49(void) ISR;

// 50 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA50(void) ISR;

// 51 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA51(void) ISR;

// 52 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA52(void) ISR;

// 53 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA53(void) ISR;

// 54 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA54(void) ISR;

// 55 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA55(void) ISR;

// 56 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA56(void) ISR;

// 57 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA57(void) ISR;

// 58 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA58(void) ISR;

// 59 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA59(void) ISR;

// 60 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA60(void) ISR;

// 61 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA61(void) ISR;

// 62 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA62(void) ISR;

// 63 TRAPA (User Vecter)
extern void WEAKALIAS INT_TRAPA63(void) ISR;

// 64 Interrupt IRQ0
extern void WEAKALIAS INT_IRQ0(void) ISR;

// 65 Interrupt IRQ1
extern void WEAKALIAS INT_IRQ1(void) ISR;

// 66 Interrupt IRQ2
extern void WEAKALIAS INT_IRQ2(void) ISR;

// 67 Interrupt IRQ3
extern void WEAKALIAS INT_IRQ3(void) ISR;

// 68 Interrupt IRQ4
extern void WEAKALIAS INT_IRQ4(void) ISR;

// 69 Interrupt IRQ5
extern void WEAKALIAS INT_IRQ5(void) ISR;

// 70 Interrupt IRQ6
extern void WEAKALIAS INT_IRQ6(void) ISR;

// 71 Interrupt IRQ7
extern void WEAKALIAS INT_IRQ7(void) ISR;

// 72 DMAC0 DEI0
extern void WEAKALIAS INT_DMAC0_DEI0(void) ISR;

// 73 Reserved

// 74 DMAC1 DEI1
extern void WEAKALIAS INT_DMAC1_DEI1(void) ISR;

// 75 Reserved

// 76 DMAC2 DEI2
extern void WEAKALIAS INT_DMAC2_DEI2(void) ISR;

// 77 Reserved

// 78 DMAC3 DEI3
extern void WEAKALIAS INT_DMAC3_DEI3(void) ISR;

// 79 Reserved

// 80 ATU01 ITV1/ITV2A/ITV2B
extern void WEAKALIAS INT_ATU01_ITV(void) ISR;

// 81 Reserved

// 82 Reserved

// 83 Reserved

// 84 ATU02 ICI0A
extern void WEAKALIAS INT_ATU02_ICI0A(void) ISR;

// 85 Reserved

// 86 ATU02 ICI0B
extern void WEAKALIAS INT_ATU02_ICI0B(void) ISR;

// 87 Reserved

// 88 ATU03 ICI0C
extern void WEAKALIAS INT_ATU03_ICI0C(void) ISR;

// 89 Reserved

// 90 ATU03 ICI0D
extern void WEAKALIAS INT_ATU03_ICI0D(void) ISR;

// 91 Reserved

// 92 ATU04 OVI0
extern void WEAKALIAS INT_ATU04_OVI0(void) ISR;

// 93 Reserved

// 94 Reserved

// 95 Reserved

// 96 ATU11 IMI1A/CMI1
extern void WEAKALIAS INT_ATU11_IMI1A(void) ISR;

// 97 ATU11 IMI1B
extern void WEAKALIAS INT_ATU11_IMI1B(void) ISR;

// 98 ATU11 IMI1C
extern void WEAKALIAS INT_ATU11_IMI1C(void) ISR;

// 99 ATU11 IMI1D
extern void WEAKALIAS INT_ATU11_IMI1D(void) ISR;

// 100 ATU12 IMI1E
extern void WEAKALIAS INT_ATU12_IMI1E(void) ISR;

// 101 ATU12 IMI1F
extern void WEAKALIAS INT_ATU12_IMI1F(void) ISR;

// 102 ATU12 IMI1G
extern void WEAKALIAS INT_ATU12_IMI1G(void) ISR;

// 103 ATU12 IMI1H
extern void WEAKALIAS INT_ATU12_IMI1H(void) ISR;

// 104 ATU13 OVI1A/OVI1B
extern void WEAKALIAS INT_ATU13_OVI1(void) ISR;

// 105 Reserved

// 106 Reserved

// 107 Reserved

// 108 ATU21 IMI2A/CMI2A
extern void WEAKALIAS INT_ATU21_IMI2A(void) ISR;

// 109 ATU21 IMI2B/CMI2B
extern void WEAKALIAS INT_ATU21_IMI2B(void) ISR;

// 110 ATU21 IMI2C/CMI2C
extern void WEAKALIAS INT_ATU21_IMI2C(void) ISR;

// 111 ATU21 IMI2D/CMI2D
extern void WEAKALIAS INT_ATU21_IMI2D(void) ISR;

// 112 ATU22 IMI2E/CMI2E
extern void WEAKALIAS INT_ATU22_IMI2E(void) ISR;

// 113 ATU22 IMI2F/CMI2F
extern void WEAKALIAS INT_ATU22_IMI2F(void) ISR;

// 114 ATU22 IMI2G/CMI2G
extern void WEAKALIAS INT_ATU22_IMI2G(void) ISR;

// 115 ATU22 IMI2H/CMI2H
extern void WEAKALIAS INT_ATU22_IMI2H(void) ISR;

// 116 ATU23 OVI2A/OVI2B
extern void WEAKALIAS INT_ATU23_OVI2(void) ISR;

// 117 Reserved

// 118 Reserved

// 119 Reserved

// 120 ATU31 IMI3A
extern void WEAKALIAS INT_ATU31_IMI3A(void) ISR;

// 121 ATU31 IMI3B
extern void WEAKALIAS INT_ATU31_IMI3B(void) ISR;

// 122 ATU31 IMI3C
extern void WEAKALIAS INT_ATU31_IMI3C(void) ISR;

// 123 ATU31 IMI3D
extern void WEAKALIAS INT_ATU31_IMI3D(void) ISR;

// 124 ATU32 OVI3
extern void WEAKALIAS INT_ATU32_OVI3(void) ISR;

// 125 Reserved

// 126 Reserved

// 127 Reserved

// 128 ATU41 IMI4A
extern void WEAKALIAS INT_ATU41_IMI4A(void) ISR;

// 129 ATU41 IMI4B
extern void WEAKALIAS INT_ATU41_IMI4B(void) ISR;

// 130 ATU41 IMI4C
extern void WEAKALIAS INT_ATU41_IMI4C(void) ISR;

// 131 ATU41 IMI4D
extern void WEAKALIAS INT_ATU41_IMI4D(void) ISR;

// 132 ATU42 OVI4
extern void WEAKALIAS INT_ATU42_OVI4(void) ISR;

// 133 Reserved

// 134 Reserved

// 135 Reserved

// 136 ATU51 IMI5A
extern void WEAKALIAS INT_ATU51_IMI5A(void) ISR;

// 137 ATU51 IMI5B
extern void WEAKALIAS INT_ATU51_IMI5B(void) ISR;

// 138 ATU51 IMI5C
extern void WEAKALIAS INT_ATU51_IMI5C(void) ISR;

// 139 ATU51 IMI5D
extern void WEAKALIAS INT_ATU51_IMI5D(void) ISR;

// 140 ATU52 OVI5
extern void WEAKALIAS INT_ATU52_OVI5(void) ISR;

// 141 Reserved

// 142 Reserved

// 143 Reserved

// 144 ATU6 CMI6A
extern void WEAKALIAS INT_ATU6_CMI6A(void) ISR;

// 145 ATU6 CMI6B
extern void WEAKALIAS INT_ATU6_CMI6B(void) ISR;

// 146 ATU6 CMI6C
extern void WEAKALIAS INT_ATU6_CMI6C(void) ISR;

// 147 ATU6 CMI6D
extern void WEAKALIAS INT_ATU6_CMI6D(void) ISR;

// 148 ATU7 CMI7A
extern void WEAKALIAS INT_ATU7_CMI7A(void) ISR;

// 149 ATU7 CMI7B
extern void WEAKALIAS INT_ATU7_CMI7B(void) ISR;

// 150 ATU7 CMI7C
extern void WEAKALIAS INT_ATU7_CMI7C(void) ISR;

// 151 ATU7 CMI7D
extern void WEAKALIAS INT_ATU7_CMI7D(void) ISR;

// 152 ATU81 OSI8A
extern void WEAKALIAS INT_ATU81_OSI8A(void) ISR;

// 153 ATU81 OSI8B
extern void WEAKALIAS INT_ATU81_OSI8B(void) ISR;

// 154 ATU81 OSI8C
extern void WEAKALIAS INT_ATU81_OSI8C(void) ISR;

// 155 ATU81 OSI8D
extern void WEAKALIAS INT_ATU81_OSI8D(void) ISR;

// 156 ATU82 OSI8E
extern void WEAKALIAS INT_ATU82_OSI8E(void) ISR;

// 157 ATU82 OSI8F
extern void WEAKALIAS INT_ATU82_OSI8F(void) ISR;

// 158 ATU82 OSI8G
extern void WEAKALIAS INT_ATU82_OSI8G(void) ISR;

// 159 ATU82 OSI8H
extern void WEAKALIAS INT_ATU82_OSI8H(void) ISR;

// 160 ATU83 OSI8I
extern void WEAKALIAS INT_ATU83_OSI8I(void) ISR;

// 161 ATU83 OSI8J
extern void WEAKALIAS INT_ATU83_OSI8J(void) ISR;

// 162 ATU83 OSI8K
extern void WEAKALIAS INT_ATU83_OSI8K(void) ISR;

// 163 ATU83 OSI8L
extern void WEAKALIAS INT_ATU83_OSI8L(void) ISR;

// 164 ATU84 OSI8M
extern void WEAKALIAS INT_ATU84_OSI8M(void) ISR;

// 165 ATU84 OSI8N
extern void WEAKALIAS INT_ATU84_OSI8N(void) ISR;

// 166 ATU84 OSI8O
extern void WEAKALIAS INT_ATU84_OSI8O(void) ISR;

// 167 ATU84 OSI8P
extern void WEAKALIAS INT_ATU84_OSI8P(void) ISR;

// 168 ATU91 CMI9A
extern void WEAKALIAS INT_ATU91_CMI9A(void) ISR;

// 169 ATU91 CMI9B
extern void WEAKALIAS INT_ATU91_CMI9B(void) ISR;

// 170 ATU91 CMI9C
extern void WEAKALIAS INT_ATU91_CMI9C(void) ISR;

// 171 ATU91 CMI9D
extern void WEAKALIAS INT_ATU91_CMI9D(void) ISR;

// 172 ATU92 CMI9E
extern void WEAKALIAS INT_ATU92_CMI9E(void) ISR;

// 173 Reserved

// 174 ATU92 CMI9F
extern void WEAKALIAS INT_ATU92_CMI9F(void) ISR;

// 175 Reserved

// 176 ATU101 CMI10A
extern void WEAKALIAS INT_ATU101_CMI10A(void) ISR;

// 177 Reserved

// 178 ATU101 CMI10B
extern void WEAKALIAS INT_ATU101_CMI10B(void) ISR;

// 179 Reserved

// 180 ATU102 ICI10A/CMI10G
extern void WEAKALIAS INT_ATU102_ICI10A(void) ISR;

// 181 Reserved

// 182 Reserved

// 183 Reserved

// 184 ATU11 IMI11A
extern void WEAKALIAS INT_ATU11_IMI11A(void) ISR;

// 185 Reserved

// 186 ATU11 IMI11B
extern void WEAKALIAS INT_ATU11_IMI11B(void) ISR;

// 187 ATU11 OVI11
extern void WEAKALIAS INT_ATU11_OVI11(void) ISR;

// 188 CMT0 CMTI0
extern void WEAKALIAS INT_CMT0_CMTI0(void) ISR;

// 189 Reserved

// 190 A/D0 ADI0
extern void WEAKALIAS INT_AD0_ADI0(void) ISR;

// 191 Reserved

// 192 CMT1 CMTI1
extern void WEAKALIAS INT_CMT1_CMTI1(void) ISR;

// 193 Reserved

// 194 A/D1 ADI1
extern void WEAKALIAS INT_AD1_ADI1(void) ISR;

// 195 Reserved

// 196 A/D2 ADI2
extern void WEAKALIAS INT_AD2_ADI2(void) ISR;

// 197 Reserved

// 198 Reserved

// 199 Reserved

// 200 SCI0 ERI0
extern void WEAKALIAS INT_SCI0_ERI0(void) ISR;

// 201 SCI0 RXI0
extern void WEAKALIAS INT_SCI0_RXI0(void) ISR;

// 202 SCI0 TXI0
extern void WEAKALIAS INT_SCI0_TXI0(void) ISR;

// 203 SCI0 TEI0
extern void WEAKALIAS INT_SCI0_TEI0(void) ISR;

// 204 SCI1 ERI1
extern void WEAKALIAS INT_SCI1_ERI1(void) ISR;

// 205 SCI1 RXI1
extern void WEAKALIAS INT_SCI1_RXI1(void) ISR;

// 206 SCI1 TXI1
extern void WEAKALIAS INT_SCI1_TXI1(void) ISR;

// 207 SCI1 TEI1
extern void WEAKALIAS INT_SCI1_TEI1(void) ISR;

// 208 SCI2 ERI2
extern void WEAKALIAS INT_SCI2_ERI2(void) ISR;

// 209 SCI2 RXI2
extern void WEAKALIAS INT_SCI2_RXI2(void) ISR;

// 210 SCI2 TXI2
extern void WEAKALIAS INT_SCI2_TXI2(void) ISR;

// 211 SCI2 TEI2
extern void WEAKALIAS INT_SCI2_TEI2(void) ISR;

// 212 SCI3 ERI3
extern void WEAKALIAS INT_SCI3_ERI3(void) ISR;

// 213 SCI3 RXI3
extern void WEAKALIAS INT_SCI3_RXI3(void) ISR;

// 214 SCI3 TXI3
extern void WEAKALIAS INT_SCI3_TXI3(void) ISR;

// 215 SCI3 TEI3
extern void WEAKALIAS INT_SCI3_TEI3(void) ISR;

// 216 SCI4 ERI4
extern void WEAKALIAS INT_SCI4_ERI4(void) ISR;

// 217 SCI4 RXI4
extern void WEAKALIAS INT_SCI4_RXI4(void) ISR;

// 218 SCI4 TXI4
extern void WEAKALIAS INT_SCI4_TXI4(void) ISR;

// 219 SCI4 TEI4
extern void WEAKALIAS INT_SCI4_TEI4(void) ISR;

// 220 HCAN0 ERS0
extern void WEAKALIAS INT_HCAN0_ERS0(void) ISR;

// 221 HCAN0 OVR0
extern void WEAKALIAS INT_HCAN0_OVR0(void) ISR;

// 222 HCAN0 RM0
extern void WEAKALIAS INT_HCAN0_RM0(void) ISR;

// 223 HCAN0 SLE0
extern void WEAKALIAS INT_HCAN0_SLE0(void) ISR;

// 224 WDT ITI
extern void WEAKALIAS INT_WDT_ITI(void) ISR;

// 225 Reserved

// 226 Reserved

// 227 Reserved

// 228 HCAN1 ERS1
extern void WEAKALIAS INT_HCAN1_ERS1(void) ISR;

// 229 HCAN1 OVR1
extern void WEAKALIAS INT_HCAN1_OVR1(void) ISR;

// 230 HCAN1 RM1
extern void WEAKALIAS INT_HCAN1_RM1(void) ISR;

// 231 HCAN1 SLE1
extern void WEAKALIAS INT_HCAN1_SLE1(void) ISR;

// 232 Reserved

// 233 Reserved

// 234 Reserved

// 235 Reserved

// 236 Reserved

// 237 Reserved

// 238 Reserved

// 239 Reserved

// 240 Reserved

// 241 Reserved

// 242 Reserved

// 243 Reserved

// 244 Reserved

// 245 Reserved

// 246 Reserved

// 247 Reserved

// 248 Reserved

// 249 Reserved

// 250 Reserved

// 251 Reserved

// 252 Reserved

// 253 Reserved

// 254 Reserved

// 255 Reserved

#endif
