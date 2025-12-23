#include <linux/sched/task.h>
#include <windows/api.h>

struct thread_info windows_thread_info;

/* should never be called ... */

void *__switch_to(struct task_struct *from, struct task_struct *to)
{
/* Let the Windows scheduler do the work: */
/* KeSetEvent(to, 1)
   KeClearEvent(from)
   KeWaitForSingleObject(from)

   ... or so ...
 */
printk("switching from %s(%d) to %s(%d) ...\n", from->comm, from->pid, to->comm, to->pid);
	return from;
}

int copy_thread(struct task_struct *p, const struct kernel_clone_args *args)
{
	p->thread_info.fn = args->fn;
	p->thread_info.fn_arg = args->fn_arg;

	return 0;
}

void flush_thread(void)
{
}

/* normally this is in linker, but now we have it here ... */
unsigned long init_stack[THREAD_SIZE / sizeof(unsigned long)];

	/* Do not start init. Do not start kthreadd. Do not
	 * start an idle loop. Just return from DriverEntry
	 * and let the host kernel (Windows) do the rest.
	 */

#if 0
void __init arch_call_rest_init(void)
{
	DbgPrint("Doing the rest ... \n");
}
#endif

/*
 * This is where the Windows thread is created. It is still 'stopped'
 * until the start_event is signalled.
 */
int arch_dup_task_struct(struct task_struct *dst, struct task_struct *src)
{
	int ret;

	*dst = *src;
	dst->thread_info.task = dst;
	ret = win_create_windows_thread(dst, &dst->thread_info.windows_thread);

	return ret;
}

