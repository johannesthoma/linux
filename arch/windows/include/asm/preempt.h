/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_PREEMPT_H
#define __ASM_PREEMPT_H

#include <linux/thread_info.h>
#include <windows/api.h>

#define PREEMPT_ENABLED	(0)

static __always_inline int preempt_count(void)
{
	return READ_ONCE(current_thread_info()->preempt_count);
}

static __always_inline volatile int *preempt_count_ptr(void)
{
	return &current_thread_info()->preempt_count;
}

enum __PREEMPT_MOD_OPT {
	PREEMPT_SET,
	PREEMPT_ADD,
	PREEMPT_SUB
};

static __always_inline int __preempt_count_modify(int val, enum __PREEMPT_MOD_OPT op)
{
	volatile int *p = preempt_count_ptr();
	int old_preempt_count = *p;

	switch (op) {
	case PREEMPT_SET: *p = val; break;
	case PREEMPT_ADD: *p += val; break;
	case PREEMPT_SUB: *p -= val; break;
	default: printk("op is %d, don't know what to do ...\n", op);
	}

printk("old preemptcount is %d new preempt count is %d p is %p\n", old_preempt_count, *p, p);
	if (old_preempt_count == PREEMPT_ENABLED && *p != PREEMPT_ENABLED)
		win_disable_preemption();
	if (old_preempt_count != PREEMPT_ENABLED && *p == PREEMPT_ENABLED)
		win_enable_preemption();

	return *p;
}

static __always_inline void preempt_count_set(int pc)
{
	__preempt_count_modify(pc, PREEMPT_SET);
}

/*
 * must be macros to avoid header recursion hell
 */
#define init_task_preempt_count(p) do { \
	task_thread_info(p)->preempt_count = FORK_PREEMPT_COUNT; \
} while (0)

#define init_idle_preempt_count(p, cpu) do { \
	task_thread_info(p)->preempt_count = PREEMPT_DISABLED; \
} while (0)

/*
#define init_task_preempt_count(p) do { \
	task_thread_info(p)->preempt_count = PREEMPT_ENABLED; \
} while (0)

#define init_idle_preempt_count(p, cpu) do { \
	task_thread_info(p)->preempt_count = PREEMPT_ENABLED; \
} while (0)
*/

static __always_inline void set_preempt_need_resched(void)
{
}

static __always_inline void clear_preempt_need_resched(void)
{
}

static __always_inline bool test_preempt_need_resched(void)
{
	return false;
}

/*
 * The various preempt_count add/sub methods
 */

static __always_inline void __preempt_count_add(int val)
{
	__preempt_count_modify(val, PREEMPT_ADD);
}

static __always_inline void __preempt_count_sub(int val)
{
	__preempt_count_modify(val, PREEMPT_SUB);
}

static __always_inline bool __preempt_count_dec_and_test(void)
{
	/*
	 * Because of load-store architectures cannot do per-cpu atomic
	 * operations; we cannot use PREEMPT_NEED_RESCHED because it might get
	 * lost.
	 */
	return !__preempt_count_modify(1, PREEMPT_SUB) && tif_need_resched();
}

/*
 * Returns true when we need to resched and can (barring IRQ state).
 */
static __always_inline bool should_resched(int preempt_offset)
{
	return unlikely(preempt_count() == preempt_offset &&
			tif_need_resched());
}

#ifdef CONFIG_PREEMPTION
extern asmlinkage void preempt_schedule(void);
extern asmlinkage void preempt_schedule_notrace(void);

#if defined(CONFIG_PREEMPT_DYNAMIC) && defined(CONFIG_HAVE_PREEMPT_DYNAMIC_KEY)

void dynamic_preempt_schedule(void);
void dynamic_preempt_schedule_notrace(void);
#define __preempt_schedule()		dynamic_preempt_schedule()
#define __preempt_schedule_notrace()	dynamic_preempt_schedule_notrace()

#else /* !CONFIG_PREEMPT_DYNAMIC || !CONFIG_HAVE_PREEMPT_DYNAMIC_KEY*/

#define __preempt_schedule() preempt_schedule()
#define __preempt_schedule_notrace() preempt_schedule_notrace()

#endif /* CONFIG_PREEMPT_DYNAMIC && CONFIG_HAVE_PREEMPT_DYNAMIC_KEY*/
#endif /* CONFIG_PREEMPTION */

#endif /* __ASM_PREEMPT_H */
