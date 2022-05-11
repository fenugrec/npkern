! For Subaru ECUs.
! Apparently they're always loaded at a fixed address in RAM : no need to move the payload
! TODO : remove auto-copy; make sure entry point is correct vs .ld file

! (c) copyright fenugrec 2022
! GPLv3
! 
! This program is free software: you can redistribute it and/or modify
! it under the terms of the GNU General Public License as published by
! the Free Software Foundation, either version 3 of the License, or
! (at your option) any later version.
! 
! This program is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
! GNU General Public License for more details.
! 
! You should have received a copy of the GNU General Public License
! along with this program.  If not, see <http://www.gnu.org/licenses/>.
!



	.list
	.section .rja
	.global RAMjump_entry

	.extern _main	!user code
	.extern _ebss
	.extern _bss
	.extern _stackinit

RAMjump_entry:
	.BALIGN 2


zero_bss:
	mov.l ebss, r1
	mov.l	bss, r0
	mov #0, r2
zero_top:
	cmp/eq	r0, r1
	bt	zero_end
	bra	zero_top
	mov.b	r2,@-r1
zero_end:

	mov.l	main,r1
	mov.l	stack,r15
	jsr     @r1
	nop

	.BALIGN 4
stack:
		.long	_stackinit
main:
		.long	_main
bss:
		.long	_sbss
ebss:
		.long	_ebss
