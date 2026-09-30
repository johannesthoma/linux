#include <asm/ptrace.h>
#include <linux/sched.h>
#include <linux/printk.h>

void show_regs(struct pt_regs *regs)
{
	printk("showing registers is not supported yet.\n");
}

void show_stack(struct task_struct *task, unsigned long *sp,
                       const char *loglvl)
{
	printk("stack backtrace printing is not supported yet.\n");
}
