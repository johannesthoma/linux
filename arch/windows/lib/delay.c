/*
 *	Precise Delay Loops for i386
 *
 *	Copyright (C) 1993 Linus Torvalds
 *	Copyright (C) 1997 Martin Mares <mj@atrey.karlin.mff.cuni.cz>
 *	Copyright (C) 2008 Jiri Hladky <hladky _dot_ jiri _at_ gmail _dot_ com>
 *
 *	The __delay function must _NOT_ be inlined as its execution time
 *	depends wildly on alignment on many x86 processors. The additional
 *	jump magic is needed to get the timing stable on all the CPU's
 *	we have to worry about.
 */

#include <linux/export.h>
#include <linux/sched.h>
#include <linux/timex.h>
#include <linux/preempt.h>
#include <linux/delay.h>

/* Taken from x86 architecture code: */
static void delay_loop(u64 __loops)
{
        unsigned long loops = (unsigned long)__loops;

        asm volatile(
                "       test %0,%0      \n"
                "       jz 3f           \n"
                "       jmp 1f          \n"

                ".align 16              \n"
                "1:     jmp 2f          \n"

                ".align 16              \n"
                "2:     dec %0          \n"
                "       jnz 2b          \n"
                "3:     dec %0          \n"

                : "+a" (loops)
                :
        );
}


void __delay(unsigned long loops)
{
	delay_loop((unsigned long long) loops*1000ULL);
}
EXPORT_SYMBOL(__delay);

noinline void __const_udelay(unsigned long xloops)
{
	__delay(++xloops);
}
EXPORT_SYMBOL(__const_udelay);

void __udelay(unsigned long usecs)
{
	__const_udelay(usecs * 0x000010c7); /* 2**32 / 1000000 (rounded up) */
}
EXPORT_SYMBOL(__udelay);

void __ndelay(unsigned long nsecs)
{
	__const_udelay(nsecs * 0x00005); /* 2**32 / 1000000000 (rounded up) */
}
EXPORT_SYMBOL(__ndelay);
