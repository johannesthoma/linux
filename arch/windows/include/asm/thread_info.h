#ifndef _ASM_THREADINFO_H
#define _ASM_THREADINFO_H

#define TIF_SIGPENDING          2       /* signal pending */
#define TIF_NEED_RESCHED        3       /* rescheduling necessary */
#define TIF_NOTIFY_SIGNAL       17      /* signal notifications exist */


struct thread_info {
	int preempt_count;
	unsigned long flags;
	struct task_struct *task;
};

#define INIT_THREAD_INFO(tsk)                   \
{                                               \
        .flags          = 0,                    \
}

static inline struct thread_info *current_thread_info(void)
{
	return NULL;
}

static __always_inline int user_mode(struct pt_regs *regs)
{
	return 0;	/* never on windows */
}

#endif
