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
	volatile unsigned tmp;

	asm volatile (
		"stc   sr,%0\n"
		"mov   %0, %1\n"
		"mov   #0xF0, r0\n"
		"extu.b r0, r0\n"	//r0 = 0000 00F0
		"and   r0, %0\n"
		"or    r0, %1\n"
		"ldc   %1, sr"
			:"=r"(val),"=r"(tmp)
			:
			:"cc","r0"
	);

	return val;
}


extern __inline__ void imask_restore(unsigned unshifted_mask) {
	volatile unsigned tmp2;
	asm volatile (
		"stc   sr,r0\n"
		"mov   #0xff,%0\n"
		"shll8 %0\n"
		"add   #0x0F, %0\n"
		"and   %0,r0\n"
		"or    %1,r0\n"
		"ldc   r0,sr"
			:"=r"(tmp2)
			:"r" (unshifted_mask)
			:"r0", "cc"
	);
}
