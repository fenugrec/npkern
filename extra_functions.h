/* Extra asm helpers for SH */

#define FUNCS_DECL static inline

/** Saves unshifted intmask and sets to 0x0F to disable.
 * Use "imask_restore()" , not set_imask() to re-enable.
 */
FUNCS_DECL unsigned imask_savedisable(void) __attribute__ ((always_inline));

/** Restore imask; assumes current imask is 0x0F
 * Only use with value obtained from imask_savedisable()
 */
FUNCS_DECL void imask_restore(unsigned unshifted_mask);


extern __inline__ unsigned imask_savedisable() {
	unsigned val;

	asm volatile (
		"stc	sr, r0 \n"
		"mov r0, %0 \n"		//save orig SR
		"or	#0xF0, r0 \n"
		"ldc	r0, sr \n"	//disable
		"mov	%0, r0 \n"
		"and	#0xF0, r0 \n"	//keep only b'IIII0000 (#imm8 is zero-extended)
		"mov	r0, %0 \n"
			:"=r"(val)
			:
			:"r0"
	);
	return val;
}


extern __inline__ void imask_restore(unsigned unshifted_mask) {
	asm volatile (
		"stc	sr, r0 \n"
		"or	#0xF0, r0 \n"	// set IIII=b'1111
		"xor #0xF0, r0 \n"	// equiv. to "R0 = (SR & 0xFFFF FF0F)"
		"or	%0, r0 \n"
		"ldc	r0, sr"
			:
			:"r" (unshifted_mask)
			:"r0"
	);
}
