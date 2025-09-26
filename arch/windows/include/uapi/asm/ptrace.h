#ifndef _ASM_PTRACE_H
#define _ASM_PTRACE_H

// see ./include/acpi/platform/acenv.h:
#define __linux__ 1

struct pt_regs {
	unsigned long ip;
	unsigned long user_sp;
};

#define task_pt_regs(p) \
        ((struct pt_regs *)(0))

static inline unsigned long instruction_pointer(struct pt_regs *regs)
{
	return regs->ip;
}

static inline unsigned long user_stack_pointer(struct pt_regs *regs)
{
	return regs->user_sp;
}

#endif
