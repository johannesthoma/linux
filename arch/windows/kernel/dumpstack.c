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

int __die(const char *str, struct pt_regs *regs, long err)
{
	printk("I would __die, but I don't know how.\n");
	return 0;
}

void die(const char *str, struct pt_regs *regs, long err)
{
	printk("I would die, but I don't know how.\n");
}

void die_addr(const char *str, struct pt_regs *regs, long err, long gp_addr)
{
	printk("I would die_addr, but I don't know how.\n");
}
