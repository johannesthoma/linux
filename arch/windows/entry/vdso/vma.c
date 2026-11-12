#include <asm/elf.h>

bool arch_syscall_is_vdso_sigreturn(struct pt_regs *regs)
{
        return false;
}


