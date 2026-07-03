#ifndef WINDOWS_PROCESSOR_H
#define WINDOWS_PROCESSOR_H

#include <asm/pgtable_types.h>

#define cpu_relax() do { } while (0)

struct thread_struct {
	unsigned long nix;
};

#define KSTK_ESP(task)         (0)
#define KSTK_EIP(task)         (0)

#define INIT_THREAD  {  }

static inline unsigned long __get_wchan(struct task_struct *p)
{
	return 0;
}

#endif
