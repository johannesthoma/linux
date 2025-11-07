#ifndef _ASM_THREADINFO_H
#define _ASM_THREADINFO_H

/* taken from um linux */
#define TIF_SYSCALL_TRACE       0       /* syscall trace active */
#define TIF_SIGPENDING          1       /* signal pending */
#define TIF_NEED_RESCHED        2       /* rescheduling necessary */
#define TIF_NOTIFY_SIGNAL       3       /* signal notifications exist */
#define TIF_RESTART_BLOCK       4
#define TIF_MEMDIE              5       /* is terminating due to OOM killer */
#define TIF_SYSCALL_AUDIT       6
#define TIF_RESTORE_SIGMASK     7
#define TIF_NOTIFY_RESUME       8
#define TIF_SECCOMP             9       /* secure computing */
#define TIF_SINGLESTEP          10      /* single stepping userspace */

#define _TIF_SYSCALL_TRACE      (1 << TIF_SYSCALL_TRACE)
#define _TIF_SIGPENDING         (1 << TIF_SIGPENDING)
#define _TIF_NEED_RESCHED       (1 << TIF_NEED_RESCHED)
#define _TIF_NOTIFY_SIGNAL      (1 << TIF_NOTIFY_SIGNAL)
#define _TIF_MEMDIE             (1 << TIF_MEMDIE)
#define _TIF_SYSCALL_AUDIT      (1 << TIF_SYSCALL_AUDIT)
#define _TIF_SECCOMP            (1 << TIF_SECCOMP)
#define _TIF_SINGLESTEP         (1 << TIF_SINGLESTEP)

struct task_struct;
extern struct task_struct init_task;

struct thread_info {
	int preempt_count;
	unsigned long flags;
	struct task_struct *task;
};

#define INIT_THREAD_INFO(tsk)                   \
{                                               \
	.preempt_count  = 0,			\
        .flags          = 0,                    \
        .task		= &tsk,			\
}

/* TODO: embed in struct task_struct and enable multithreading. */
extern struct thread_info windows_thread_info;

static inline struct thread_info *current_thread_info(void)
{
	windows_thread_info.task = &init_task;

	return &windows_thread_info;
}

static __always_inline int user_mode(struct pt_regs *regs)
{
	return 0;	/* never on windows */
}

#endif
