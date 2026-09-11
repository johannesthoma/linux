#include <linux/sched.h>

long arch_ptrace(struct task_struct *child, long request,
                 unsigned long addr, unsigned long data)
{
	return -EINVAL;
}

void ptrace_disable(struct task_struct *child)
{
}

const struct user_regset_view *task_user_regset_view(struct task_struct *task)
{
	printk("task_user_regset_view task: %p not implemented\n", task);
	return NULL;
}

