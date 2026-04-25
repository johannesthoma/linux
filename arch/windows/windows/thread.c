#include <linux/sched.h>
#include <linux/list.h>
#include <linux/kthread.h>
#include <windows/api.h>

#include "internal.h"
#if 0
/* Those are defined in linux/types.h */
#define _UINTPTR_T_DEFINED 1
#define _INTPTR_T_DEFINED 1

#include <wdm.h>
#endif

/* TODO: spin lock !!!! */
/* TODO: hashmap !!!! */

struct thread_info *find_current_thread_info(struct _KTHREAD *windows_thread)
{
	struct task_struct *t;

	list_for_each_entry(t, &init_task.tasks, tasks) {
		if (t->thread_info.windows_thread == windows_thread)
			return &t->thread_info;
	}
	return &init_task.thread_info;
}

struct thread_info *win_find_current_thread_info(void)
{
	return find_current_thread_info(KeGetCurrentThread());
}

#ifdef CONFIG_HAVE_KERNEL_STACKSWAP_ENABLE
extern BOOLEAN KeSetKernelStackSwapEnable(BOOLEAN Enable);
#endif

static void __attribute__((stdcall)) win_thread_setup(void *targ)
{
	struct task_struct *t = targ;
	int ret;
	NTSTATUS status;

	int (*threadfn)(void *);
	void *data;

		/* Linux never swaps out kernel stack areas. This
		 * should fix a very rare list corruption in a
		 * wake_up() call (the list contained an element
		 * that was on a stack that was swapped out, causing
		 * list corruption).
		 */

#ifdef CONFIG_HAVE_KERNEL_STACKSWAP_ENABLE
	KeSetKernelStackSwapEnable(FALSE);
#endif

        status = KeWaitForSingleObject(t->thread_info.task_queued_event, Executive, KernelMode, FALSE, (PLARGE_INTEGER)NULL);
        if (!NT_SUCCESS(status)) {
		WARN_ONCE(1, "win_thread_setup: KeWaitForSingleObject returned %08X\n", status);
		goto out;
	}
	threadfn = t->thread_info.fn;
	data = t->thread_info.fn_arg;

	if (threadfn) {
		ret = threadfn(data);

		WARN_ONCE(ret != 0, "Thread %s returned non-zero exit status. Ignored, since Windows threads are void.\n", t->comm);
		WARN_ONCE(KeGetCurrentIrql() > PASSIVE_LEVEL, "Warning: IRQL is %d when exiting thread. System will posibly lockup.\n", KeGetCurrentIrql());
	} else {
		WARN_ONCE(1, "not a kthread function, also no fn in thread_info, giving up ...\n");
	}

out:
		/* According to Microsoft docs we must not exit a thread
		 * with stack swapping disabled, so enable it here again.
		 */
#ifdef CONFIG_HAVE_KERNEL_STACKSWAP_ENABLE
	KeSetKernelStackSwapEnable(TRUE);
#endif
}

	/* This has to be called (at least) on the init_task struct */

int win_initialize_task_queued_event(struct task_struct *task)
{
	task->thread_info.task_queued_event = win_allocate_memory(sizeof(struct _KEVENT));
	if (task->thread_info.task_queued_event == NULL)
		return -ENOMEM;

	KeInitializeEvent(task->thread_info.task_queued_event, SynchronizationEvent, FALSE);
	return 0;
}

int win_create_windows_thread(struct task_struct *task, struct _KTHREAD **thread_object_p)
{
	HANDLE h;
	NTSTATUS status;
	int retries;
	int ret;

	ret = win_initialize_task_queued_event(task);
	if (ret < 0)
		return ret;

	retries = 0;
	while (1) {
	        status = PsCreateSystemThread(&h, THREAD_ALL_ACCESS, NULL, NULL, NULL, win_thread_setup, (void*) task);
		if (NT_SUCCESS(status))
			break;

		if (status != STATUS_INSUFFICIENT_RESOURCES)
			return -EINVAL;	/* TODO: or so ... */

		return -ENOMEM;

/* TODO: later:
                if (KeGetCurrentIrql() == PASSIVE_LEVEL)
			msleep(100);

		retries++;
*/
	}

	if (thread_object_p)
	        status = ObReferenceObjectByHandle(h, THREAD_ALL_ACCESS, NULL, KernelMode, (void**) thread_object_p, NULL);

	ZwClose(h);
	return 0;
}

	/* TODO: call this somewhere ... free_task or so */
	/* Update: call this in kthread_join() */
int win_cleanup_windows_thread(void *thread_object)
{
	NTSTATUS status;

        status = KeWaitForSingleObject(thread_object, Executive, KernelMode, FALSE, (PLARGE_INTEGER)NULL);

        if (!NT_SUCCESS(status)) {
                printk("KeWaitForSingleObject failed with status %x\n", status);
		return -EINVAL;
	}
        ObDereferenceObject(thread_object);

	return 0;
}

void win_wake_up_task(struct task_struct *t)
{
	KeSetEvent(t->thread_info.task_queued_event, 0, FALSE);
}

void win_put_task_to_sleep(struct task_struct *t)
{
	NTSTATUS status;

	if (t->thread_info.preempt_count != 0)
		win_enable_preemption();

        status = KeWaitForSingleObject(t->thread_info.task_queued_event, Executive, KernelMode, FALSE, (PLARGE_INTEGER)NULL);
	if (!NT_SUCCESS(status))
		WARN_ONCE(1, "win_put_task_to_sleep: KeWaitForSingleObject returned %08X\n", status);

	/* ok woken up, continue execution */
	if (t->thread_info.preempt_count != 0)
		win_disable_preemption();
}

void win_set_realtime_priority(struct task_struct *t)
{
	if (t == NULL)
		return;

	KeSetPriorityThread(t->thread_info.windows_thread, LOW_REALTIME_PRIORITY);
}
