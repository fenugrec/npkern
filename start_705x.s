! based on KPIT GNUSH
! This moves the whole payload up to the desired address,

! (c) copyright fenugrec 2016
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

	.extern _rja_start
	.extern _main	!user code
	.extern _ebss
	.extern _bss
	.extern _stackinit
	.extern _endpayload

RAMjump_entry:
	mova rj_plus4, r0
	mov	#0, r3		!flag : 0 on first iteration (moving the microkernel), then 1 during normal payload copying.
rj_plus4:
	! here r0 == ( &RAMjump + 4 ) == &rj_plus4 (at the ramjump location, not the intended destination) the first time this is run.
		! move payload from *r0 to *r5, skipping first 4 bytes (doesn't matter)
	mov.l	rja_start_adj, r5
second_iter:
	mov.l endpl, r4

	tst	r3, r3
	bf	move_pl	!BNZ	!skip the following if we're on the second iteration

		! else, copy only this microkernel, then jump to it at its desired location.
	mov.l	p_move_done_2, r4

		!simulation says 11 cycles per loop here
move_pl:
	cmp/hi	r5, r4		!(r4 >= r5 ?)
	mov.l		@r0+, r2
	mov.l		r2, @r5
	bt/s		move_pl
	add		#4, r5
move_done:
		!if we were just copying the microkernel, jump to it.
	tst	r3, r3
	bf	move_done_2	!BNZ
	mov.l	p_second_iter, r2
	jsr	@r2
	mov	#1, r3

	.BALIGN 4
rja_start_adj:
	.long _rja_start + 4
endpl:
		.long _endpayload
p_second_iter:
		.long second_iter
p_move_done_2:
		.long move_done_2


	.BALIGN 2
move_done_2:

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
