/* Source : KPIT GNU SH distribution, with modifications fenugrec 2016-2022
 * WARNING !!
 * original implementation had buggy implementations for set_imask() and others,
 * where the inline asm was missing some registers in the clobberlist.
 *
 * Reordered, and declarations filtered with ifdefs to reduce warnings,
 * Added missing implementations:
	sleep
	gbr_write_byte
	gbr_write_long
	gbr_read_word
	gbr_read_long
 *
 * Writing inline asm blocks is tricky.
 *
 * Note re "asm volatile" : "
		GCC’s optimizers sometimes discard asm statements if they determine there is no need for the output variables. Also, the optimizers may move code out of loops if they believe that the code will always return the same result. [...] Using the volatile qualifier disables these optimizations. "
 *
 *
 */

#define trapa(trap_no)	asm ("trapa  %0"::"i"(trap_no))


#define FUNCS_DECL static inline
FUNCS_DECL void set_imask(unsigned long mask) __attribute__ ((always_inline));
FUNCS_DECL int get_imask(void) __attribute__ ((always_inline));
FUNCS_DECL void set_vbr(void *base) __attribute__ ((always_inline));
FUNCS_DECL void *get_vbr(void) __attribute__ ((always_inline));

FUNCS_DECL void nop(void) __attribute__ ((always_inline));

#if BROKEN_ASM
// see other BROKEN_ASM block later in file

FUNCS_DECL void set_cr(int cr) __attribute__ ((always_inline));
FUNCS_DECL int get_cr(void) __attribute__ ((always_inline));

FUNCS_DECL void *get_gbr(void) __attribute__ ((always_inline));
FUNCS_DECL void set_gbr(void *base) __attribute__ ((always_inline));

FUNCS_DECL unsigned char gbr_read_byte(int offset) __attribute__ ((always_inline));
FUNCS_DECL unsigned short gbr_read_word(int offset) __attribute__ ((always_inline));
FUNCS_DECL unsigned long gbr_read_long(int offset) __attribute__ ((always_inline));

FUNCS_DECL void gbr_write_byte(int offset, unsigned char data) __attribute__ ((always_inline));
FUNCS_DECL void gbr_write_word(int offset, unsigned short data) __attribute__ ((always_inline));
FUNCS_DECL void gbr_write_long(int offset, unsigned long data) __attribute__ ((always_inline));

FUNCS_DECL void sleep(void) __attribute__ ((always_inline));

#if defined __SH3__ || defined __SH4__ || defined __SH2A__
FUNCS_DECL void tas(char *addr) __attribute__ ((always_inline));
FUNCS_DECL void trace(long v) __attribute__ ((always_inline));
FUNCS_DECL void prefetch (void *p) __attribute__ ((always_inline));
#endif



#if defined __SH3__ || defined __SH4__
FUNCS_DECL void ldtlb(void) __attribute__ ((always_inline));
#endif



#ifndef __SH1__
FUNCS_DECL unsigned long dmuls_l(long data1, long data2) __attribute__ ((always_inline));
FUNCS_DECL long dmuls_h(long data1, long data2) __attribute__ ((always_inline));
FUNCS_DECL int macl(int *ptr1,int*ptr2,unsigned int count) __attribute__ ((always_inline));
FUNCS_DECL int macll(int *ptr1,int*ptr2,unsigned int count,unsigned int mask) __attribute__ ((always_inline));
#endif

FUNCS_DECL unsigned long swapw(unsigned long data) __attribute__ ((always_inline));
FUNCS_DECL unsigned long end_cnvl(unsigned long data) __attribute__ ((always_inline));

#if !defined (__SH2A_NOFPU__)
#if defined (__SH2A__) || defined (__SH2E__) || defined (__SH3E__) || defined(__SH4_SINGLE__) ||defined(__SH4__) || defined(__SH4_SINGLE_ONLY__)
FUNCS_DECL void set_fpscr(int cr) __attribute__ ((always_inline));
FUNCS_DECL int get_fpscr() __attribute__ ((always_inline));
#endif
#endif

#ifdef __SH4__
FUNCS_DECL float fipr(float vect1[4], float vect2[4]) __attribute__ ((always_inline));
FUNCS_DECL void ftrv(float vec1[4],float vec2[4]) __attribute__ ((always_inline));
FUNCS_DECL void ftrvsub(float vec1[4],float vec2[4],float vec3[4]) __attribute__ ((always_inline));

FUNCS_DECL void mtrx4mul(float mat1[4][4], float mat2[4][4]) __attribute__ ((always_inline));
FUNCS_DECL void mtrx4mulsub(float mat1[4][4],float mat2[4][4],float mat3[4][4]) __attribute__ ((always_inline));
FUNCS_DECL void add4(float vec1[4],float vec2[4],float vec3[4]) __attribute__ ((always_inline));

FUNCS_DECL void ld_ext( float mat[4][4]) __attribute__ ((always_inline));
FUNCS_DECL void st_ext(float mat[4][4]) __attribute__ ((always_inline));
#endif

#ifdef __SH2A__
FUNCS_DECL long clips_byte (long var) __attribute__ ((always_inline));
FUNCS_DECL long clips_word (long var) __attribute__ ((always_inline));
FUNCS_DECL unsigned long clipu_byte (unsigned long var) __attribute__ ((always_inline));
FUNCS_DECL unsigned long clipu_word (unsigned long var) __attribute__ ((always_inline));
FUNCS_DECL void set_tbr(void *tbr) __attribute__ ((always_inline));
FUNCS_DECL void * get_tbr(void) __attribute__ ((always_inline));
FUNCS_DECL void stbank(long data, int rn, int bn) __attribute__ ((always_inline));
FUNCS_DECL long ldbank(int rn, int bn) __attribute__ ((always_inline));
#endif
#endif

extern __inline__ void nop(void)
{
	asm volatile ("nop");
}


/** get current stack pointer (r15) */
FUNCS_DECL int get_sp(void)__attribute__ ((always_inline));
extern __inline__ int get_sp(void)
{
	long val;
	asm("mov r15, %0":"=r"(val));
	return val;
}

extern __inline__ void set_imask(unsigned long mask)
{
	mask <<= 4;
	mask &= 0xf0;

	asm volatile (
		"stc	sr, r0 \n"
		"or	#0xF0, r0 \n"	// set IIII=b'1111
		"xor	#0xF0, r0 \n"	// equiv. to "R0 = (SR & 0xFFFF FF0F)"
		"or	%0,r0\n"
		"ldc	r0,sr"
			:
			:"r" (mask)
			:"r0"
	);

}

extern __inline__ int get_imask()
{
	volatile int val;
	asm volatile (
		"stc         sr,r0\n"
		"shlr2       r0\n"
		"shlr2       r0\n"
		"and         #15,r0\n"
		"mov r0,%0"
			:"=r"(val)
			:
			:"r0"
	);
	return val;
}
extern __inline__ void set_vbr(void *vbr)
{
	asm volatile (
		"mov %0, r2\n"
		"ldc r2,vbr"
			:
			:"r"(vbr)
			:"r2"
	);
}
extern __inline__ void* get_vbr(void)
{
	void *ptr;
	asm volatile (
		"stc vbr,r2\n"
		"mov.l r2, %0"
			:"=m"(ptr)
			:
			:"r2"
	);
	return ptr;
}



#if BROKEN_ASM
/* these need to be double-checked for :
 * - clobber lists
 * - written as single asm volatile (...) blocks
*/

#define gbr_and_byte(offset,mask) {asm("mov %0, r0"::"r"(offset):"r0");asm("and.b %0, @(r0,gbr)"::"g"(mask));}


extern __inline__ void set_cr(int cr)
{
	asm("mov %0, r0"::"r"(cr):"r0");
	asm("ldc r0, sr");
}


extern __inline__ int get_cr(void)
{
	long val;
	asm("stc sr, r0":::"r0");
	asm("mov r0, %0":"=r"(val));
	return val;
}

extern __inline__ void set_gbr(void *gbr)
{
	asm("mov %0, r2"::"r"(gbr):"r2");
	asm("ldc r2, gbr");
}
extern __inline__ void* get_gbr(void)
{
	void *ptr;
	asm("stc gbr,r2":::"r2");
	asm("mov.l r2, %0":"=m"(ptr));
	return ptr;
}
extern __inline__ unsigned char gbr_read_byte(int offset)
{
	unsigned char val;
	asm("mov %0, r1"::"r"(offset):"r1");
	asm("stc gbr, r2":::"r2");
	asm("add r1, r2");
	asm("mov.b @r2, r0");
	asm("extu.b r0, r2");
	asm("mov.l r2, %0":"=m"(val));
	return val;
}
extern __inline__ unsigned short gbr_read_word(int offset)
{
	unsigned short val;
	asm("mov %0, r1"::"r"(offset):"r1");
	asm("stc gbr, r2":::"r2");
	asm("add r1, r2");
	asm("mov.w @r2, r0");
	asm("extu.w r0, r2");
	asm("mov.l r2, %0":"=m"(val));
	return val;
}
extern __inline__ unsigned long gbr_read_long(int offset)
{
	unsigned long val;
	asm("mov %0, r1"::"r"(offset):"r1");
	asm("stc gbr, r2":::"r2");
	asm("add r1, r2");
	asm("mov.l @r2, r0");
	asm("mov.l r0, %0":"=m"(val));
	return val;
}
extern __inline__ void gbr_write_byte(int offset, unsigned char data)
{
	asm("mov %0, r1"::"r"(offset):"r1");
	asm("stc gbr, r2":::"r2");
	asm("add r1, r2");
	asm("mov %0,r1"::"r"(data));
	asm("mov r1,r0");
	asm("mov.b r0,@r2");
}
extern __inline__ void gbr_write_word(int offset, unsigned short data)
{
	asm("mov %0, r1"::"r"(offset):"r1");
	asm("stc gbr, r2":::"r2");
	asm("add r1, r2");
	asm("mov %0,r1"::"r"(data));
	asm("mov r1,r0");
	asm("mov.w r0,@r2");
}
extern __inline__ void gbr_write_long(int offset, unsigned long data)
{
	asm("mov %0, r1"::"r"(offset):"r1");
	asm("stc gbr, r2":::"r2");
	asm("add r1, r2");
	asm("mov %0,r1"::"r"(data));
	asm("mov r1,r0");
	asm("mov.l r0,@r2");
}
extern __inline__ void sleep(void)
{
      asm("sleep");
}

#if defined __SH3__ || defined __SH4__ || defined __SH2A__
extern __inline__ void tas(char *addr)
{
	asm("mov %0, r1"::"r"(addr):"r1");
	asm("tas.b @r1");
}


extern __inline__ void prefetch (void *p)
{
	asm("mov %0, r2"::"r"(p):"r2");
	asm("pref @r2");
}
#endif

#if defined __SH3__ || defined __SH4__
extern __inline__ void ldtlb(void)
{
	asm("ldtlb");
}
#endif

#ifndef __SH1__
extern __inline__ unsigned long dmuls_l(long data1, long data2)
{
	unsigned long val;
	asm("mov %0, r4"::"r"(data1):"r4");
	asm("mov %0, r5"::"r"(data2):"r5");
	asm("dmuls.l r4,r5");
	asm("sts macl,r6":::"r6");
	asm("sts mach,r2":::"r2");
	asm("lds r6,macl");
	asm("sts macl,r2");
	asm("mov.l r2, %0":"=m"(val));
	return val;
}
extern __inline__ long dmuls_h(long data1,long data2)
{
	unsigned long val;
	asm("mov %0, r4"::"r"(data1):"r4");
	asm("mov %0, r5"::"r"(data2):"r5");
	asm("dmuls.l r4,r5");
	asm("sts macl,r6":::"r6");
	asm("sts mach,r2":::"r2");
	asm("lds r2,mach");
	asm("sts mach,r6");
	asm("mov.l r6, %0":"=m"(val));
	return val;
}
extern __inline__ int macl(int *ptr1,int*ptr2,unsigned int count)
{
	int val;
	asm("mov %0,r4"::"r"(count):"r4");
	asm("clrmac");
	asm("sts macl,r2":::"r2");
	asm("sts mach,r6":::"r6");
	asm("tst r4,r4");
	asm("bt 2f");
	asm("mov %0, r6"::"r"(ptr1):"r6");
	asm("mov %0, r5"::"r"(ptr2):"r5");
	asm("shll2 r4");
	asm("add r6,r4");
	asm("1:");
	asm("lds r2,macl");
	asm("mac.l @r6+,@r5+");
	asm("sts macl,r2");
	asm("cmp/hi r6,r4");
	asm("bt 1b");
	asm("2:");
	asm("lds r2,macl");
	asm("sts macl,r6");
	asm("mov.l r6, %0":"=m"(val));
	return val;
}
extern __inline__ int macll(int *ptr1,int*ptr2,unsigned int count,unsigned int mask)
{
	int val;
	asm("mov %0,r4"::"r"(count):"r4");
	asm("clrmac");
	asm("sts macl,r2":::"r2");
	asm("sts mach,r6":::"r6");
	asm("tst r4,r4");
	asm("bt 1f");
	asm("mov %0, r6"::"r"(ptr1):"r6");
	asm("mov %0, r5"::"r"(ptr2):"r5");
	asm("mov %0, r7"::"r"(mask):"r7");
	asm("shll2 r4");
	asm("add r6,r4");
	asm("2:");
	asm("lds r2,macl");
	asm("mac.l @r6+,@r5+");
	asm("sts macl,r2");
	asm("and r7,r5");
	asm("cmp/hi r6,r4");
	asm("bt 2b");
	asm("1:");
	asm("lds r2,macl");
	asm("sts macl,r6");
	asm("mov.l r6, %0":"=m"(val));
	return val;
}

#endif

extern __inline__ unsigned long swapw(unsigned long data)
{
	unsigned long val;
	asm("mov %0, r2"::"r"(data):"r2");
	asm("swap.w r2,r6":::"r6");
	asm("mov.l r6, %0":"=m"(val));
	return val;
}
extern __inline__ unsigned long end_cnvl(unsigned long data)
{
	unsigned long val;
	asm("mov %0, r6"::"r"(data):"r6");
	asm("mov r6,r2":::"r2");
	asm("swap.b r2,r6");
	asm("swap.w r6,r2");
	asm("swap.b r2,r6");
	asm("mov.l r6, %0":"=m"(val));
	return val;
}

#if !defined (__SH2A_NOFPU__)
#if defined (__SH2A__) || defined (__SH2E__) || defined (__SH3E__) || defined(__SH4_SINGLE__) ||defined(__SH4__) || defined(__SH4_SINGLE_ONLY__)

extern __inline__ void set_fpscr(int cr)
{
	asm("mov %0, r2"::"r"(cr):"r2");
	asm("lds r2,fpscr");
}
extern __inline__ int get_fpscr()
{
	int val;
	asm("sts fpscr, r2":::"r2");
	asm("mov.l r2, %0":"=m"(val));
	return val;
}
#endif
#endif

#ifdef __SH4__
extern __inline__ float fipr(float vect1[4], float vect2[4])
{
	float val;
	asm("mov %0,r2"::"r"(vect1):"r2");
	asm("fmov.s @r2+,fr0":::"fr0");
	asm("fmov.s @r2+,fr1":::"fr1");
	asm("fmov.s @r2+,fr2":::"fr2");
	asm("fmov.s @r2+,fr3":::"fr3");
	asm("add #-16,r2");
	asm("fmov fr3,fr8":::"fr8");
	asm("fmov fr2,fr9":::"fr9");
	asm("mov %0, r2"::"r"(vect2):"r2");
	asm("fmov.s @r2+,fr4":::"fr4");
	asm("fmov.s @r2+,fr5":::"fr5");
	asm("fmov.s @r2+,fr6":::"fr6");
	asm("fmov.s @r2+,fr7":::"fr7");
	asm("add #-16,r2");
	asm("fmov fr9,fr2");
	asm("fmov fr8,fr3");
	asm("fipr fv4,fv0");
	asm("fmov fr3,fr8");
	asm("fmov fr8, %0":"=f"(val));
	return val;
}
extern __inline__ void ld_ext( float mat[4][4])
{
	asm("mov %0, r2"::"r"(mat):"r2");
	asm("frchg");
	asm("fmov.s @r2+,fr0":::"fr0");
	asm("fmov.s @r2+,fr1":::"fr1");
	asm("fmov.s @r2+,fr2":::"fr2");
	asm("fmov.s @r2+,fr3":::"fr3");
	asm("fmov.s @r2+,fr4":::"fr4");
	asm("fmov.s @r2+,fr5":::"fr5");
	asm("fmov.s @r2+,fr6":::"fr6");
	asm("fmov.s @r2+,fr7":::"fr7");
	asm("fmov.s @r2+,fr8":::"fr8");
	asm("fmov.s @r2+,fr9":::"fr9");
	asm("fmov.s @r2+,fr10":::"fr10");
	asm("fmov.s @r2+,fr11":::"fr11");
	asm("fmov.s @r2+,fr12":::"fr12");
	asm("fmov.s @r2+,fr13":::"fr13");
	asm("fmov.s @r2+,fr14":::"fr14");
	asm("fmov.s @r2+,fr15":::"fr15");
	asm("frchg");
	asm("add #-64,r2");
}
extern __inline__ void st_ext(float mat[4][4])
{
	asm("mov %0,r2"::"r"(mat):"r2");
	asm("add #64,r2");
	asm("frchg");
	asm("fmov.s fr15,@-r2");
	asm("fmov.s fr14,@-r2");
	asm("fmov.s fr13,@-r2");
	asm("fmov.s fr12,@-r2");
	asm("fmov.s fr11,@-r2");
	asm("fmov.s fr10,@-r2");
	asm("fmov.s fr9,@-r2");
	asm("fmov.s fr8,@-r2");
	asm("fmov.s fr7,@-r2");
	asm("fmov.s fr6,@-r2");
	asm("fmov.s fr5,@-r2");
	asm("fmov.s fr4,@-r2");
	asm("fmov.s fr3,@-r2");
	asm("fmov.s fr2,@-r2");
	asm("fmov.s fr1,@-r2");
	asm("fmov.s fr0,@-r2");
	asm("frchg");
}
extern __inline__ void ftrv(float vec1[4],float vec2[4])
{
	asm("mov %0,r6"::"r"(vec1):"r6");
	asm("fmov.s @r6+,fr0");
	asm("fmov.s @r6+,fr1");
	asm("fmov.s @r6+,fr2");
	asm("fmov.s @r6+,fr3");
	asm("add #-16,r6");
	asm("fmov fr3,fr9");
	asm("fmov fr2,fr8");
	asm("fmov fr8,fr2");
	asm("fmov fr9,fr3");
	asm("ftrv xmtrx,fv0");
	asm("fmov fr3,fr9");
	asm("fmov fr2,fr8");
	asm("mov %0,r6"::"r"(vec2));
	asm("fmov fr8,fr2");
	asm("fmov fr9,fr3");
	asm("add #16,r6");
	asm("fmov.s fr3,@-r6");
	asm("fmov.s fr2,@-r6");
	asm("fmov.s fr1,@-r6");
	asm("fmov.s fr0,@-r6");
}
extern __inline__ void ftrvsub(float vec1[4],float vec2[4],float vec3[4])
{
	asm("mov %0,r2"::"r"(vec1):"r2");
	asm("fmov.s @r2+,fr0");
	asm("fmov.s @r2+,fr1");
	asm("fmov.s @r2+,fr2");
	asm("fmov.s @r2+,fr3");
	asm("add #-16,r2");
	asm("fmov fr3,fr9");
	asm("fmov fr2,fr8");
	asm("fmov fr8,fr2");
	asm("fmov fr9,fr3");
	asm("ftrv xmtrx,fv0");
	asm("fmov fr3,fr9");
	asm("fmov fr2,fr8");
	asm("mov %0,r2"::"r"(vec2));
	asm("fmov.s @r2+,fr4");
	asm("fmov.s @r2+,fr5");
	asm("fmov.s @r2+,fr6");
	asm("fmov.s @r2+,fr7");
	asm("add #-16,r2");
	asm("fsub fr4,fr0");
	asm("fsub fr5,fr1");
	asm("fmov fr8,fr2");
	asm("fsub fr6,fr2");
	asm("fmov fr2,fr8");
	asm("fmov fr9,fr3");
	asm("fsub fr7,fr3");
	asm("fmov fr3,fr9");
	asm("mov %0,r2"::"r"(vec3));
	asm("fmov fr8,fr2");
	asm("fmov fr9,fr3");
	asm("add #16,r2");
	asm("fmov.s fr3,@-r2");
	asm("fmov.s fr2,@-r2");
	asm("fmov.s fr1,@-r2");
	asm("fmov.s fr0,@-r2");
}
extern __inline__ void add4(float vec1[4],float vec2[4],float vec3[4])
{
	asm("mov %0,r2"::"r"(vec1):"r2");
	asm("fmov.s @r2+,fr0");
	asm("fmov.s @r2+,fr1");
	asm("fmov.s @r2+,fr2");
	asm("fmov.s @r2+,fr3");
	asm("add #-16,r2");
	asm("fmov fr3,fr9");
	asm("fmov fr2,fr8");
	asm("mov %0,r2"::"r"(vec2));
	asm("fmov.s @r2+,fr4");
	asm("fmov.s @r2+,fr5");
	asm("fmov.s @r2+,fr6");
	asm("fmov.s @r2+,fr7");
	asm("add #-16,r2");
	asm("fadd fr4,fr0");
	asm("fadd fr5,fr1");
	asm("fmov fr8,fr2");
	asm("fadd fr6,fr2");
	asm("fmov fr2,fr8");
	asm("fmov fr9,fr3");
	asm("fadd fr7,fr3");
	asm("fmov fr3,fr9");
	asm("mov %0,r2"::"r"(vec3));
	asm("fmov fr8,fr2");
	asm("fmov fr9,fr3");
	asm("add #16,r2");
	asm("fmov.s fr3,@-r2");
	asm("fmov.s fr2,@-r2");
	asm("fmov.s fr1,@-r2");
	asm("fmov.s fr0,@-r2");
}
extern __inline__ void mtrx4mul(float mat1[4][4], float mat2[4][4])
{
	asm("mov %0,r2"::"r"(mat1):"r2");
	asm("mov %0,r6"::"r"(mat2):"r6");
	asm("fmov.s @r2+,fr0");
	asm("fmov.s @r2+,fr1");
	asm("fmov.s @r2+,fr2");
	asm("fmov.s @r2+,fr3");
	asm("fmov fr3,fr8");
	asm("fmov fr2,fr9");
	asm("fmov fr9,fr2");
	asm("fmov fr8,fr3");
	asm("ftrv xmtrx,fv0");
	asm("fmov fr3,fr8");
	asm("fmov fr2,fr9");
	asm("fmov fr9,fr2");
	asm("fmov fr8,fr3");
	asm("add #16,r6");
	asm("fmov.s fr3,@-r6");
	asm("fmov.s fr2,@-r6");
	asm("fmov.s fr1,@-r6");
	asm("fmov.s fr0,@-r6");
	asm("fmov.s @r2+,fr0");
	asm("fmov.s @r2+,fr1");
	asm("fmov.s @r2+,fr2");
	asm("fmov.s @r2+,fr3");
	asm("fmov fr3,fr8");
	asm("fmov fr2,fr9");
	asm("fmov fr9,fr2");
	asm("fmov fr8,fr3");
	asm("ftrv xmtrx,fv0");
	asm("fmov fr3,fr8");
	asm("fmov fr2,fr9");
	asm("add #32,r6");
	asm("fmov fr9,fr2");
	asm("fmov fr8,fr3");
	asm("fmov.s fr3,@-r6");
	asm("fmov.s fr2,@-r6");
	asm("fmov.s fr1,@-r6");
	asm("fmov.s fr0,@-r6");
	asm("fmov.s @r2+,fr0");
	asm("fmov.s @r2+,fr1");
	asm("fmov.s @r2+,fr2");
	asm("fmov.s @r2+,fr3");
	asm("fmov fr3,fr8");
	asm("fmov fr2,fr9");
	asm("fmov fr9,fr2");
	asm("fmov fr8,fr3");
	asm("ftrv xmtrx,fv0");
	asm("fmov fr3,fr8");
	asm("fmov fr2,fr9");
	asm("add #32,r6");
	asm("fmov fr9,fr2");
	asm("fmov fr8,fr3");
	asm("fmov.s fr3,@-r6");
	asm("fmov.s fr2,@-r6");
	asm("fmov.s fr1,@-r6");
	asm("fmov.s fr0,@-r6");
	asm("fmov.s @r2+,fr0");
	asm("fmov.s @r2+,fr1");
	asm("fmov.s @r2+,fr2");
	asm("fmov.s @r2+,fr3");
	asm("fmov fr3,fr8");
	asm("fmov fr2,fr9");
	asm("fmov fr9,fr2");
	asm("fmov fr8,fr3");
	asm("ftrv xmtrx,fv0");
	asm("fmov fr3,fr8");
	asm("fmov fr2,fr9");
	asm("add #32,r6");
	asm("fmov fr9,fr2");
	asm("fmov fr8,fr3");
	asm("fmov.s fr3,@-r6");
	asm("fmov.s fr2,@-r6");
	asm("fmov.s fr1,@-r6");
	asm("fmov.s fr0,@-r6");
}
extern __inline__ void mtrx4mulsub(float mat1[4][4],float mat2[4][4],float mat3[4][4])
{
	asm("mov %0,r6"::"r"(mat1):"r6");
	asm("mov %0,r2"::"r"(mat2):"r2");
	asm("mov %0,r5"::"r"(mat3):"r5");
	asm("fmov.s      @r6+,fr0");
	asm("fmov.s      @r6+,fr1");
	asm("fmov.s      @r6+,fr2");
	asm("fmov.s      @r6+,fr3");
	asm("fmov      fr3,fr9");
	asm("fmov      fr2,fr8");
	asm("fmov      fr8,fr2");
	asm("fmov      fr9,fr3");
	asm("ftrv        xmtrx,fv0");
	asm("fmov      fr3,fr9");
	asm("fmov      fr2,fr8");
	asm("fmov.s      @r2+,fr4");
	asm("fmov.s      @r2+,fr5");
	asm("fmov.s      @r2+,fr6");
	asm("fmov.s      @r2+,fr7");
	asm("fsub        fr4,fr0");
	asm("fsub        fr5,fr1");
	asm("fmov      fr8,fr2");
	asm("fsub        fr6,fr2");
	asm("fmov      fr2,fr8");
	asm("fmov      fr9,fr3");
	asm("fsub        fr7,fr3");
	asm("fmov      fr3,fr9");
	asm("fmov      fr8,fr2");
	asm("fmov      fr9,fr3");
	asm("add         #16,r5");
	asm("fmov.s      fr3,@-r5");
	asm("fmov.s      fr2,@-r5");
	asm("fmov.s      fr1,@-r5");
	asm("fmov.s      fr0,@-r5");
	asm("fmov.s      @r6+,fr0");
	asm("fmov.s      @r6+,fr1");
	asm("fmov.s      @r6+,fr2");
	asm("fmov.s      @r6+,fr3");
	asm("fmov      fr3,fr9");
	asm("fmov      fr2,fr8");
	asm("fmov      fr8,fr2");
	asm("fmov      fr9,fr3");
	asm("ftrv        xmtrx,fv0");
	asm("fmov      fr3,fr9");
	asm("fmov      fr2,fr8");
	asm("fmov.s      @r2+,fr4");
	asm("fmov.s      @r2+,fr5");
	asm("fmov.s      @r2+,fr6");
	asm("fmov.s      @r2+,fr7");
	asm("fsub        fr4,fr0");
	asm("fsub        fr5,fr1");
	asm("fmov      fr8,fr2");
	asm("fsub        fr6,fr2");
	asm("fmov      fr2,fr8");
	asm("fmov      fr9,fr3");
	asm("fsub        fr7,fr3");
	asm("fmov      fr3,fr9");
	asm("add         #32,r5");
	asm("fmov      fr8,fr2");
	asm("fmov      fr9,fr3");
	asm("fmov.s      fr3,@-r5");
	asm("fmov.s      fr2,@-r5");
	asm("fmov.s      fr1,@-r5");
	asm("fmov.s      fr0,@-r5");
	asm("fmov.s      @r6+,fr0");
	asm("fmov.s      @r6+,fr1");
	asm("fmov.s      @r6+,fr2");
	asm("fmov.s      @r6+,fr3");
	asm("fmov      fr3,fr9");
	asm("fmov      fr2,fr8");
	asm("fmov      fr8,fr2");
	asm("fmov      fr9,fr3");
	asm("ftrv        xmtrx,fv0");
	asm("fmov      fr3,fr9");
	asm("fmov      fr2,fr8");
	asm("fmov.s      @r2+,fr4");
	asm("fmov.s      @r2+,fr5");
	asm("fmov.s      @r2+,fr6");
	asm("fmov.s      @r2+,fr7");
	asm("fsub        fr4,fr0");
	asm("fsub        fr5,fr1");
	asm("fmov      fr8,fr2");
	asm("fsub        fr6,fr2");
	asm("fmov      fr2,fr8");
	asm("fmov      fr9,fr3");
	asm("fsub        fr7,fr3");
	asm("fmov      fr3,fr9");
	asm("add         #32,r5");
	asm("fmov      fr8,fr2");
	asm("fmov      fr9,fr3");
	asm("fmov.s      fr3,@-r5");
	asm("fmov.s      fr2,@-r5");
	asm("fmov.s      fr1,@-r5");
	asm("fmov.s      fr0,@-r5");
	asm("fmov.s      @r6+,fr0");
	asm("fmov.s      @r6+,fr1");
	asm("fmov.s      @r6+,fr2");
	asm("fmov.s      @r6+,fr3");
	asm("fmov      fr3,fr9");
	asm("fmov      fr2,fr8");
	asm("fmov      fr8,fr2");
	asm("fmov      fr9,fr3");
	asm("ftrv        xmtrx,fv0");
	asm("fmov      fr3,fr9");
	asm("fmov      fr2,fr8");
	asm("fmov.s      @r2+,fr4");
	asm("fmov.s      @r2+,fr5");
	asm("fmov.s      @r2+,fr6");
	asm("fmov.s      @r2+,fr7");
	asm("fsub        fr4,fr0");
	asm("fsub        fr5,fr1");
	asm("fmov      fr8,fr2");
	asm("fsub        fr6,fr2");
	asm("fmov      fr2,fr8");
	asm("fmov      fr9,fr3");
	asm("fsub        fr7,fr3");
	asm("fmov      fr3,fr9");
	asm("add         #32,r5");
	asm("fmov      fr8,fr2");
	asm("fmov      fr9,fr3");
	asm("fmov.s      fr3,@-r5");
	asm("fmov.s      fr2,@-r5");
	asm("fmov.s      fr1,@-r5");
	asm("fmov.s      fr0,@-r5");
}
#endif

#ifdef __SH2A__


/*
This function preforms Signed saturation value comparison operation for 1-byte data.

The value is stored in general register Rn, the saturation upper-limit value
is stored in general register Rn if the contents of Rn exceed the saturation
upper-limit value, or the saturation lower-limit value is stored in Rn if the
contents of Rn are less than the saturation lowerlimit value,
and the CS bit is set to 1.

*/

extern __inline__ long clips_byte(long var)
{
	volatile long result;
	asm ("mov %0,r2"::"r"(var):"r2");
	asm ("clips.b r2");
	asm ("mov r2,%0":"=r"(result):);
	return result;
}


/*
This function preforms Signed saturation value comparison operation for 2-byte data.
The value is stored in general register Rn, the saturation upper-limit value
is stored in general register Rn if the contents of Rn exceed the saturation
upper-limit value, or the saturation lower-limit value is stored in Rn if the
contents of Rn are less than the saturation lowerlimit value,
and the CS bit is set to 1.

*/

extern __inline__ long clips_word(long var)
{
	volatile long result;
	asm ("mov %0,r3"::"r"(var):"r3");
	asm ("clips.w r3");
	asm ("mov r3,%0":"=r"(result):);
	return result;
}



/*
This function preforms Unsigned saturation value comparison operation for 1-byte data.
The value is stored in general register Rn, the saturation upper-limit value
is stored in general register Rn if the contents of Rn exceed the saturation
upper-limit value, or the saturation lower-limit value is stored in Rn if the
contents of Rn are less than the saturation lowerlimit value,
and the CS bit is set to 1.

*/

extern __inline__ unsigned long clipu_byte(unsigned long var)
{
	volatile unsigned long result;
	asm ("mov %0,r4"::"r"(var):"r4");
	asm ("clipu.b r4");
	asm ("mov r4,%0":"=r"(result):);
	return result;
}



/*
This function preforms Unsigned saturation value comparison operation for 2-byte data.
The value is stored in general register Rn, the saturation upper-limit value
is stored in general register Rn if the contents of Rn exceed the saturation
upper-limit value, or the saturation lower-limit value is stored in Rn if the
contents of Rn are less than the saturation lowerlimit value,
and the CS bit is set to 1.

*/
extern __inline__ unsigned long clipu_word(unsigned long var)
{
	volatile unsigned long result;
	asm ("mov %0,r5"::"r"(var):"r5");
	asm ("clipu.w r5");
	asm ("mov r5,%0":"=r"(result) :);
	return result;
}


/*
This function sets the value of the TBR register to the value specified
by the user using the variable tbr

*/

extern __inline__ void set_tbr(void *tbr)
{
	asm("mov %0,r6"::"r"(tbr):"r6");
	asm("ldc r6,tbr");
}


/*
This function gets the value of the TBR register to the specified variable.

*/
extern __inline__ void * get_tbr(void)
{
	void *ptr;
	asm("stc tbr,r7":::"r7");
	asm("mov r7,%0":"=r"(ptr));
	return ptr;
}


/*
This is a register-bank related function. The function stores the specified value
to the bank, if bank number and register number in the bank are specified by the user.

*/
extern __inline__ void stbank(long data, int rn, int bn)
{
	volatile long address=0;   // Push POP not required from GNUSH V12.01 onwards
//	asm("mov.l r0,@-r15");
	asm("mov %0,r0"::"r"(data):"r0");       //user data transfered to general reg R0.
	rn<<=2;                                 //calculation of register address
	address |= rn;
	bn<<=7;                                 //calculation of bank address
	address |= bn;                          //The final address where it data is to be stored
	asm("mov %0,r8"::"r"(address):"r8");
	asm("stbank r0,@r8");
//	asm("mov.l @r15+,r0");
}



/*
This is a register-bank related function. The function gets the value from the bank,
if bank number and register number in the bank are specified by the user

*/
extern __inline__ long ldbank(int rn, int bn)
{
	volatile long address=0,result;
	rn<<=2;									//calculation of register address
	address |= rn;
	bn<<=7;									//calculation of bank address
	address |= bn;							//The final address where data is.
	asm("mov %0,r9"::"r"(address):"r9");
	asm("ldbank @r9,r0");
	asm("mov r0,%0":"=r"(result):);
	return result;
}

#endif
#endif