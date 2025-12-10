#include <linux/spinlock_types.h>
#include <linux/printk.h>

/* Those are defined in linux/types.h */
#define _UINTPTR_T_DEFINED 1
#define _INTPTR_T_DEFINED 1

#include <ntdef.h>
#include <ntddk.h>

void win_disable_preemption(void)
{
	KIRQL old_irql;

	KeRaiseIrql(DISPATCH_LEVEL, &old_irql);
	if (old_irql != PASSIVE_LEVEL)
		printk("win_disable_preemption: irql was %d, expected PASSIVE_LEVEL (=0)\n", old_irql);
}

void win_enable_preemption(void)
{
	/* This might BSOD if the old irql in win_disable_preemption() was
	 * not == PASSIVE_LEVEL
	 */
	KeLowerIrql(PASSIVE_LEVEL);
}

