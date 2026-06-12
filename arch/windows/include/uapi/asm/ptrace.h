#ifndef _ASM_PTRACE_H
#define _ASM_PTRACE_H

#include <asm/ptrace-abi.h>

// see ./include/acpi/platform/acenv.h:
#define __linux__ 1

struct pt_regs {
	/*
	 * NB: 32-bit x86 CPUs are inconsistent as what happens in the
	 * following cases (where %seg represents a segment register):
	 *
	 * - pushl %seg: some do a 16-bit write and leave the high
	 *   bits alone
	 * - movl %seg, [mem]: some do a 16-bit write despite the movl
	 * - IDT entry: some (e.g. 486) will leave the high bits of CS
	 *   and (if applicable) SS undefined.
	 *
	 * Fortunately, x86-32 doesn't read the high bits on POP or IRET,
	 * so we can just treat all of the segment registers as 16-bit
	 * values.
	 */
	unsigned long bx;
	unsigned long cx;
	unsigned long dx;
	unsigned long si;
	unsigned long di;
	unsigned long bp;
	unsigned long ax;
	unsigned short ds;
	unsigned short __dsh;
	unsigned short es;
	unsigned short __esh;
	unsigned short fs;
	unsigned short __fsh;
	/*
	 * On interrupt, gs and __gsh store the vector number.  They never
	 * store gs any more.
	 */
	unsigned short gs;
	unsigned short __gsh;
	/* On interrupt, this is the error code. */
	unsigned long orig_ax;
	unsigned long ip;
	unsigned short cs;
	unsigned short __csh;
	unsigned long flags;
	unsigned long sp;
	unsigned short ss;
	unsigned short __ssh;
};

// TODO: !!!!
#define task_pt_regs(p) \
        ((struct pt_regs *)(0))

static inline unsigned long instruction_pointer(struct pt_regs *regs)
{
	return regs->ip;
}

static inline unsigned long user_stack_pointer(struct pt_regs *regs)
{
	return regs->sp;
}

#endif
