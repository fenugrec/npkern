#ifndef _IVECT_7050_H
#define _IVECT_7050_H
/***********************************************************************/
/* 7050/7051 vectors */
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



#define ISR_attrib __attribute__ ((interrupt_handler))


/** Define vector number for each */
#define	IVTN_PowerON_Reset_PC	(0x00000000 / 4)
#define IVTN_POR_SP	(0x00000004 / 4)
#define	IVTN_Manual_Reset_PC	(0x00000008 / 4)
#define IVTN_MR_SP	(0x0000000C / 4)
#define	IVTN_INT_Illegal_code	(0x00000010 / 4)
#define	IVTN_INT_Illegal_slot	(0x00000018 / 4)
#define	IVTN_INT_CPU_Address	(0x00000024 / 4)
#define	IVTN_INT_DMAC_Address	(0x00000028 / 4)
#define	IVTN_INT_NMI	(0x0000002C / 4)
#define	IVTN_INT_User_Break	(0x00000030 / 4)
#define	IVTN_INT_TRAPA32	(0x00000080 / 4)
#define	IVTN_INT_TRAPA33	(0x00000084 / 4)
#define	IVTN_INT_TRAPA34	(0x00000088 / 4)
#define	IVTN_INT_TRAPA35	(0x0000008c / 4)
#define	IVTN_INT_TRAPA36	(0x00000090 / 4)
#define	IVTN_INT_TRAPA37	(0x00000094 / 4)
#define	IVTN_INT_TRAPA38	(0x00000098 / 4)
#define	IVTN_INT_TRAPA39	(0x0000009c / 4)
#define	IVTN_INT_TRAPA40	(0x000000a0 / 4)
#define	IVTN_INT_TRAPA41	(0x000000a4 / 4)
#define	IVTN_INT_TRAPA42	(0x000000a8 / 4)
#define	IVTN_INT_TRAPA43	(0x000000ac / 4)
#define	IVTN_INT_TRAPA44	(0x000000b0 / 4)
#define	IVTN_INT_TRAPA45	(0x000000b4 / 4)
#define	IVTN_INT_TRAPA46	(0x000000b8 / 4)
#define	IVTN_INT_TRAPA47	(0x000000bc / 4)
#define	IVTN_INT_TRAPA48	(0x000000c0 / 4)
#define	IVTN_INT_TRAPA49	(0x000000c4 / 4)
#define	IVTN_INT_TRAPA50	(0x000000c8 / 4)
#define	IVTN_INT_TRAPA51	(0x000000cc / 4)
#define	IVTN_INT_TRAPA52	(0x000000d0 / 4)
#define	IVTN_INT_TRAPA53	(0x000000d4 / 4)
#define	IVTN_INT_TRAPA54	(0x000000d8 / 4)
#define	IVTN_INT_TRAPA55	(0x000000dc / 4)
#define	IVTN_INT_TRAPA56	(0x000000e0 / 4)
#define	IVTN_INT_TRAPA57	(0x000000e4 / 4)
#define	IVTN_INT_TRAPA58	(0x000000e8 / 4)
#define	IVTN_INT_TRAPA59	(0x000000ec / 4)
#define	IVTN_INT_TRAPA60	(0x000000f0 / 4)
#define	IVTN_INT_TRAPA61	(0x000000f4 / 4)
#define	IVTN_INT_TRAPA62	(0x000000f8 / 4)
#define	IVTN_INT_TRAPA63	(0x000000fc / 4)
#define	IVTN_INT_IRQ0	(0x00000100 / 4)
#define	IVTN_INT_IRQ1	(0x00000104 / 4)
#define	IVTN_INT_IRQ2	(0x00000108 / 4)
#define	IVTN_INT_IRQ3	(0x0000010c / 4)
#define	IVTN_INT_IRQ4	(0x00000110 / 4)
#define	IVTN_INT_IRQ5	(0x00000114 / 4)
#define	IVTN_INT_IRQ6	(0x00000118 / 4)
#define	IVTN_INT_IRQ7	(0x0000011c / 4)
#define	IVTN_INT_DMAC0_DEI0	(0x00000120 / 4)
#define	IVTN_INT_DMAC1_DEI1	(0x00000128 / 4)
#define	IVTN_INT_DMAC2_DEI2	(0x00000130 / 4)
#define	IVTN_INT_DMAC3_DEI3	(0x00000138 / 4)
#define	IVTN_INT_ATU01_ITV	(0x00000140 / 4)
#define	IVTN_INT_ATU02_ICI0A	(0x00000150 / 4)
#define	IVTN_INT_ATU02_ICI0B	(0x00000154 / 4)
#define	IVTN_INT_ATU02_ICI0C	(0x00000158 / 4)
#define	IVTN_INT_ATU02_ICI0D	(0x0000015c / 4)
#define	IVTN_INT_ATU03_OV1O	(0x00000160 / 4)
#define	IVTN_INT_ATU11_IMI1A	(0x00000170 / 4)
#define	IVTN_INT_ATU11_IMI1B	(0x00000174 / 4)
#define	IVTN_INT_ATU11_IMI1C	(0x00000178 / 4)
#define	IVTN_INT_ATU12_IMI1D	(0x00000180 / 4)
#define	IVTN_INT_ATU12_IMI1E	(0x00000184 / 4)
#define	IVTN_INT_ATU12_IMI1F	(0x00000188 / 4)
#define	IVTN_INT_ATU13_OV11	(0x00000190 / 4)
#define	IVTN_INT_ATU2_IMI2A	(0x000001a0 / 4)
#define	IVTN_INT_ATU2_IMI2B	(0x000001a4 / 4)
#define	IVTN_INT_ATU2_OV12	(0x000001a8 / 4)
#define	IVTN_INT_ATU31_IMI3A	(0x000001b0 / 4)
#define	IVTN_INT_ATU31_IMI3B	(0x000001b4 / 4)
#define	IVTN_INT_ATU31_IMI3C	(0x000001b8 / 4)
#define	IVTN_INT_ATU31_IMI3D	(0x000001bc / 4)
#define	IVTN_INT_ATU32_OV13	(0x000001c0 / 4)
#define	IVTN_INT_ATU41_IMI4A	(0x000001d0 / 4)
#define	IVTN_INT_ATU41_IMI4B	(0x000001d4 / 4)
#define	IVTN_INT_ATU41_IMI4C	(0x000001d8 / 4)
#define	IVTN_INT_ATU41_IMI4D	(0x000001dc / 4)
#define	IVTN_INT_ATU42_OV14	(0x000001e0 / 4)
#define	IVTN_INT_ATU5_IMI5A	(0x000001f0 / 4)
#define	IVTN_INT_ATU5_IMI5B	(0x000001f4 / 4)
#define	IVTN_INT_ATU5_OV15	(0x000001f8 / 4)
#define	IVTN_INT_ATU6_CMI6	(0x00000200 / 4)
#define	IVTN_INT_ATU7_CMI7	(0x00000204 / 4)
#define	IVTN_INT_ATU8_CMI8	(0x00000208 / 4)
#define	IVTN_INT_ATU9_CMI9	(0x0000020c / 4)
#define	IVTN_INT_ATU101_OSI10A	(0x00000210 / 4)
#define	IVTN_INT_ATU101_OSI10B	(0x00000214 / 4)
#define	IVTN_INT_ATU101_OSI10C	(0x00000218 / 4)
#define	IVTN_INT_ATU102_OSI10D	(0x00000220 / 4)
#define	IVTN_INT_ATU102_OSI10E	(0x00000224 / 4)
#define	IVTN_INT_ATU102_OSI10F	(0x00000228 / 4)
#define	IVTN_INT_ATU103_OSI10G	(0x00000230 / 4)
#define	IVTN_INT_ATU103_OSI10H	(0x00000234 / 4)
#define	IVTN_INT_CMT0_CMTI0	(0x00000240 / 4)
#define	IVTN_INT_AD0_ADI0	(0x00000244 / 4)
#define	IVTN_INT_CMT1_CMTI1	(0x00000250 / 4)
#define	IVTN_INT_AD1_ADI1	(0x00000254 / 4)
#define	IVTN_INT_SCI0_ERI0	(0x00000260 / 4)
#define	IVTN_INT_SCI0_RXI0	(0x00000264 / 4)
#define	IVTN_INT_SCI0_TXI0	(0x00000268 / 4)
#define	IVTN_INT_SCI0_TEI0	(0x0000026c / 4)
#define	IVTN_INT_SCI1_ERI1	(0x00000270 / 4)
#define	IVTN_INT_SCI1_RXI1	(0x00000274 / 4)
#define	IVTN_INT_SCI1_TXI1	(0x00000278 / 4)
#define	IVTN_INT_SCI1_TEI1	(0x0000027c / 4)
#define	IVTN_INT_SCI2_ERI2	(0x00000280 / 4)
#define	IVTN_INT_SCI2_RXI2	(0x00000284 / 4)
#define	IVTN_INT_SCI2_TXI2	(0x00000288 / 4)
#define	IVTN_INT_SCI2_TEI2	(0x0000028c / 4)
#define	IVTN_INT_WDT_ITI	(0x00000290 / 4)



#endif
