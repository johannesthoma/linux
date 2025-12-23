#include <linux/sched.h>
#include <linux/list.h>
#include <linux/kthread.h>
#include <windows/api.h>

/* Those are defined in linux/types.h */
#define _UINTPTR_T_DEFINED 1
#define _INTPTR_T_DEFINED 1

// #include <ntdef.h>
// #include <ntddk.h>
#include <wdm.h>

/* TODO: spin lock !!!! */
struct thread_info *find_current_thread_info(struct _KTHREAD *windows_thread)
{
	struct task_struct *t;

	list_for_each_entry(t, &init_task.tasks, tasks) {
		if (t->thread_info.windows_thread == windows_thread)
{
// printk("returning thread %s@%p ...\n", t->comm, t);
			return &t->thread_info;
}
	}
// printk("returning init_task ...\n");
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

	printk(KERN_DEBUG "About to start thread %s\n", t->comm);
		/* Linux never swaps out kernel stack areas. This
		 * should fix a very rare list corruption in a
		 * wake_up() call (the list contained an element
		 * that was on a stack that was swapped out, causing
		 * list corruption).
		 */

#ifdef CONFIG_HAVE_KERNEL_STACKSWAP_ENABLE
	KeSetKernelStackSwapEnable(FALSE);
#endif

		/* t->windows_thread may be still invalid here, do not
		 * printk().
		 */

        status = KeWaitForSingleObject(t->thread_info.task_queued_event, Executive, KernelMode, FALSE, (PLARGE_INTEGER)NULL);
        if (!NT_SUCCESS(status)) {
		printk("On waiting for start event: KeWaitForSingleObject failed with status %x\n", status);

#ifdef CONFIG_HAVE_KERNEL_STACKSWAP_ENABLE
		KeSetKernelStackSwapEnable(TRUE);
#endif
		return;
	}
		/* TODO: needed? It is "auto-clear" (SynchronizationEvent) */
	KeClearEvent(t->thread_info.task_queued_event);
//	printk(KERN_DEBUG "thread %s woken up ...\n", t->comm);

	/* TODO: do we need this? */
/*
	int (*threadfn)(void *data) = kthread_func(t);
	void *data = kthread_data(t);
*/
	threadfn = t->thread_info.fn;
	data = t->thread_info.fn_arg;

	if (threadfn) {
		ret = threadfn(data);

		if (ret != 0)
			printk(KERN_WARNING "Thread %s returned non-zero exit status. Ignored, since Windows threads are void.\n", t->comm);

		if (KeGetCurrentIrql() > PASSIVE_LEVEL)
			printk("Warning: IRQL is %d when exiting thread. System will posibly lockup.\n", KeGetCurrentIrql());
	} else {
		printk("not a kthread function, also no fn in thread_info, giving up ...\n");
	}

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


	/* Again, we try to be more close to the Linux kernel API.
	 * This really creates and starts the thread created earlier
	 * by kthread_create() as a windows kernel thread. If the
	 * start process should fail, -1 is returned (which is
	 * different from the Linux kernel API, sorry for that...)
	 * Else same as in Linux: 0: task is already running (yes,
	 * you can call this multiple times, but since there is no
	 * way to temporarily stop a windows kernel thread, always
	 * 0 is returned) or 1: task was started.
	 */

void win_wake_up_task(struct task_struct *t)
{
//	printk("waking up %s ...\n", t->comm);
	KeSetEvent(t->thread_info.task_queued_event, 0, FALSE);
}

void win_put_task_to_sleep(struct task_struct *t)
{
	NTSTATUS status;

//	printk("putting %s to sleep ...\n", t->comm);
//	KeClearEvent(t->thread_info.task_queued_event);
//	printk("task %s preempt_count is %d...\n", t->comm, t->thread_info.preempt_count);
	if (t->thread_info.preempt_count != 0)
		win_enable_preemption();
	/* sleep */
        status = KeWaitForSingleObject(t->thread_info.task_queued_event, Executive, KernelMode, FALSE, (PLARGE_INTEGER)NULL);
        if (!NT_SUCCESS(status)) {
		printk("KeWaitForSingleObject returned %08X\n", status);
	}
//	printk("%s woken up, continuing ...\n", t->comm);
	/* Clear event here, in case we got woken up while we are running ... */
//	printk("%s clearing event ...\n", t->comm);
	KeClearEvent(t->thread_info.task_queued_event);
//	printk("task %s preempt_count is %d...\n", t->comm, t->thread_info.preempt_count);
	/* ok woken up, continue execution */
	if (t->thread_info.preempt_count != 0)
		win_disable_preemption();
}

	/* Creates a new task_struct, but start the thread (by
	 * calling PsCreateSystemThread()). Thread will wait for
	 * start event which is signalled by wake_up_process(struct
	 * task_struct) later.
	 *
	 * If PsCreateSystemThread should fail this returns an
	 * ERR_PTR(-ENOMEM)
	 *
	 * This now 'emulates' Linux behaviour such that no changes
	 * to driver code should be neccessary (at least not in the
	 * DRBD code).
	 */

#if 0
	/* Use this to create a task_struct for a Windows thread
	 * This is needed so we can call wait_event_XXX functions
	 * within those threads.
	 */

struct task_struct *make_me_a_windrbd_thread(const char *name, ...)
{
	struct task_struct *t;
	KIRQL flags;
	va_list args;
	int i;

	if ((t = kzalloc(sizeof(*t), GFP_KERNEL)) == NULL)
		return ERR_PTR(-ENOMEM);

		/* The thread will be created later in wake_up_process(),
		 * since Windows doesn't know of threads that are stopped
		 * when created.
		 */

	t->windows_thread = KeGetCurrentThread();
	spin_lock_init(&t->thread_started_lock);

//	KeInitializeEvent(&t->sig_event, NotificationEvent, FALSE);
	KeInitializeEvent(&t->sig_event, SynchronizationEvent, FALSE);
	KeInitializeEvent(&t->start_event, SynchronizationEvent, FALSE);
	t->has_sig_event = TRUE;
	t->sig = -1;
	t->is_root = 0;

	va_start(args, name);
	i = _vsnprintf(t->comm, sizeof(t->comm)-1, name, args);
	va_end(args);
	if (i == -1) {
		kfree(t);
		return ERR_PTR(-ERANGE);
	}

	spin_lock_irqsave(&next_pid_lock, flags);
	next_pid++;
	t->pid = next_pid;
	spin_unlock_irqrestore(&next_pid_lock, flags);

#ifdef CONFIG_HAVE_KERNEL_STACKSWAP_ENABLE
	KeSetKernelStackSwapEnable(FALSE);
#endif

	spin_lock_irqsave(&thread_list_lock, flags);
	list_add(&t->list, &thread_list);
	spin_unlock_irqrestore(&thread_list_lock, flags);

	return t;
}

	/* Call this when a thread returns to the calling Windows
	 * kernel function. This is mandatory since we enable
	 * stack swapping in here again.
	 */

void return_to_windows(struct task_struct *t)
{
	KIRQL flags;

#ifdef CONFIG_HAVE_KERNEL_STACKSWAP_ENABLE
	KeSetKernelStackSwapEnable(TRUE);
#endif


	spin_lock_irqsave(&thread_list_lock, flags);
	list_del(&t->list);
	spin_unlock_irqrestore(&thread_list_lock, flags);
	kfree(t);
}

#endif

void win_set_realtime_priority(struct task_struct *t)
{
	if (t == NULL)
		return;

	KeSetPriorityThread(t->thread_info.windows_thread, LOW_REALTIME_PRIORITY);
}

#if 0
void sudo(void)
{
	if (is_windrbd_thread(current))
		current->is_root = 1;
}
#endif

