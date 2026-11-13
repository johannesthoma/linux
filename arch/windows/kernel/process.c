#include <linux/sched/task.h>
#include <windows/api.h>

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/cpu.h>
#include <linux/prctl.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/sched/idle.h>
#include <linux/sched/debug.h>
#include <linux/sched/task.h>
#include <linux/sched/task_stack.h>
#include <linux/init.h>
#include <linux/export.h>
#include <linux/pm.h>
#include <linux/tick.h>
#include <linux/random.h>
#include <linux/user-return-notifier.h>
#include <linux/dmi.h>
#include <linux/utsname.h>
#include <linux/stackprotector.h>
#include <linux/cpuidle.h>
#include <linux/acpi.h>
#include <linux/elf-randomize.h>
#include <linux/static_call.h>
#include <trace/events/power.h>
#include <linux/hw_breakpoint.h>
#include <linux/entry-common.h>
#include <asm/cpu.h>
#include <asm/apic.h>
#include <linux/uaccess.h>
#include <asm/mwait.h>
#include <asm/fpu/api.h>
#include <asm/fpu/sched.h>
#include <asm/fpu/xstate.h>
#include <asm/debugreg.h>
#include <asm/nmi.h>
#include <asm/tlbflush.h>
#include <asm/mce.h>
#include <asm/vm86.h>
#include <asm/switch_to.h>
#include <asm/desc.h>
#include <asm/prctl.h>
#include <asm/spec-ctrl.h>
#include <asm/io_bitmap.h>
#include <asm/proto.h>
#include <asm/frame.h>
#include <asm/unwind.h>
#include <asm/tdx.h>
#include <asm/mmu_context.h>
#include <asm/shstk.h>

#include "process.h"

/*
 * per-CPU TSS segments. Threads are completely 'soft' on Linux,
 * no more per-task TSS's. The TSS size is kept cacheline-aligned
 * so they are allowed to end up in the .data..cacheline_aligned
 * section. Since TSS's are completely CPU-local, we want them
 * on exact cacheline boundaries, to eliminate cacheline ping-pong.
 */
__visible DEFINE_PER_CPU_PAGE_ALIGNED(struct tss_struct, cpu_tss_rw) = {
	.x86_tss = {
		/*
		 * .sp0 is only used when entering ring 0 from a lower
		 * privilege level.  Since the init task never runs anything
		 * but ring 0 code, there is no need for a valid value here.
		 * Poison it.
		 */
		.sp0 = (1UL << (BITS_PER_LONG-1)) + 1,

#ifdef CONFIG_X86_32
		.sp1 = TOP_OF_INIT_STACK,

		.ss0 = __KERNEL_DS,
		.ss1 = __KERNEL_CS,
#endif
		.io_bitmap_base	= IO_BITMAP_OFFSET_INVALID,
	 },
};
EXPORT_PER_CPU_SYMBOL(cpu_tss_rw);

struct thread_info windows_thread_info;

/* should never be called ... */

struct task_struct *__switch_to(struct task_struct *from, struct task_struct *to)
{
/* Let the Windows scheduler do the work: */
/* KeSetEvent(to, 1)
   KeClearEvent(from)
   KeWaitForSingleObject(from)

   ... or so ...
 */
	BUG();
// printk("switching from %s(%d) to %s(%d) ...\n", from->comm, from->pid, to->comm, to->pid);
	return from;
}

void
start_thread(struct pt_regs *regs, unsigned long new_ip, unsigned long new_sp)
{
	printk("start_thread new_ip: %08x new_sp: %08x\n", new_ip, new_sp);
#if 0
        loadsegment(gs, 0);
        regs->fs                = 0;
        regs->ds                = __USER_DS;
        regs->es                = __USER_DS;
        regs->ss                = __USER_DS;
        regs->cs                = __USER_CS;
        regs->ip                = new_ip;
        regs->sp                = new_sp;
        regs->flags             = X86_EFLAGS_IF;
#endif
}
EXPORT_SYMBOL_GPL(start_thread);

u32 elf_hwcap2 = 0;

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

