#include <linux/sched.h>

long arch_ptrace(struct task_struct *child, long request,
                 unsigned long addr, unsigned long data)
{
	return -EINVAL;
}

void ptrace_disable(struct task_struct *child)
{
}

