/*
*****************************************************************************
**
** Linker script for SH7051 (350nm) kernel, running from RAM.
**	- no heap
**	- stack at end of RAM
**
From GNU ld docs :
"
Every loadable or allocatable output section has two addresses. The ?rst is the VMA, or
virtual memory address. This is the address the section will have when the output ?le is
run. The second is the LMA, or load memory address. This is the address at which the
section will be loaded. In most cases the two addresses will be the same. An example of
when they might be diferent is when a data section is loaded into ROM, and then copied
into RAM when the program starts up (this technique is often used to initialize global
variables in a ROM based system). In this case the ROM address would be the LMA, and
the RAM address would be the VMA.
"
ADDR(section) returns the VMA of <section>.
LOADADDR(section) returns the LMA of <section>
*****************************************************************************
*/

/* (c) copyright fenugrec 2017
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



/* Entry Point */
ENTRY(RAMjump_entry)

MEMORY {
	RAM (xw)	: ORIGIN = 0xFFFFD800, LENGTH = 10K
	RMETA (xr) : ORIGIN = 0xFFFFD800, LENGTH = 64
	/* skip the area @ FFFFD800 because there's some metadata copied there */
	RJFIX (xw)	: ORIGIN = 0xFFFFD840, LENGTH = 6200

}
REGION_ALIAS("TGT", RJFIX);

/* Highest address of the user mode stack */
_stackinit =  ORIGIN(RAM) + LENGTH(RAM) - 4;

/* Define output sections */
SECTIONS
{
	/* program code and other data */
	.text :
	{
		_rja_start = .;	/* where the whole payload must be moved */
		. = ALIGN(4);
		*(.rja)
		*(.text)           /* .text sections (code) */
		*(.text*)          /* .text* sections (code) */

		. = ALIGN(4);
		_etext = .;        /* define a global symbols at end of code */
	} >TGT

	/* Constant data  */
	.rodata :
	{
		. = ALIGN(4);
		*(.rodata)         /* .rodata sections (constants, strings, etc.) */
		*(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
		. = ALIGN(4);
	} >TGT


	/* Initialized data sections */
	.data :
	{
		. = ALIGN(4);
		_sdata = .;        /* create a global symbol at data start */
		*(.data)           /* .data sections */
		*(.data*)          /* .data* sections */

		. = ALIGN(4);
		_edata = .;        /* define a global symbol at data end */
		_idatalen = . - _sdata;
	} >TGT

ASSERT(_sdata == _edata, "unhandled initialized data !")

	/* Uninitialized data section */
	. = ALIGN(4);
	.bss :
	{
		_sbss = .;         /* define a global symbol at bss start */
		*(.bss)
		*(.bss*)
		*(COMMON)

		. = ALIGN(4);
		_ebss = .;         /* define a global symbol at bss end */
		_bsslen = . - _sbss;
		_endpayload = .;
	} >TGT


	/* Remove information from the standard libraries */
	/DISCARD/ :
	{
	*(.comment)
	libc.a ( * )
	libm.a ( * )
	libgcc.a ( * )
	}

}
