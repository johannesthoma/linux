#ifndef _ASM_PTRACE_H
#define _ASM_PTRACE_H

// see ./include/acpi/platform/acenv.h:
#define __linux__ 1

struct pt_regs {
	unsigned long x;
};

#define task_pt_regs(p) \
        ((struct pt_regs *)(0))

#endif
