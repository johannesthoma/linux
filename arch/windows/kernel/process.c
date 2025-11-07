#include <linux/sched/task.h>

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
	return from;
}

int copy_thread(struct task_struct *p, const struct kernel_clone_args *args)
{
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

void __init arch_call_rest_init(void)
{
	DbgPrint("Doing the rest ... \n");
}

