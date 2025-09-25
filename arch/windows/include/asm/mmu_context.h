struct mm_struct;

static inline void switch_mm(struct mm_struct *prev, struct mm_struct *next,
                             struct task_struct *tsk)
{
	/* nothing */
}

#include <asm-generic/mmu_context.h>

