#include <linux/errno.h>
#include <linux/sched.h>
#include <windows/api.h>

int arch_early_irq_init(void)
{
	return 0;
}

unsigned int arch_dynirq_lower_bound(unsigned int from)
{
	return from;
}

unsigned long arch_local_save_flags(void)
{
	return 0;
}

void arch_local_irq_restore(unsigned long flags)
{
}

int arch_prctl_spec_ctrl_get(struct task_struct *t, unsigned long which)
{
        return -EINVAL;
}

const char *arch_vma_name(/* struct vm_area_struct *vma */ void *x)
{
	return NULL;
}

struct win_timer_list *timer_interrupt;

void timer_interrupt_handler(struct win_timer_list *t)
{
//	DbgPrint("tick %lld ...\n", jiffies);
	jiffies++;
	win_mod_timer(timer_interrupt, 1);
}

void __init init_IRQ(void)
{
	timer_interrupt = win_allocate_timer(timer_interrupt_handler);
	win_mod_timer(timer_interrupt, 1);
}
