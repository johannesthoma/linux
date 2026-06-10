#ifndef _ASM_CURRENT_H
#define _ASM_CURRENT_H

#ifndef __ASSEMBLY__

struct thread_info;
extern struct thread_info *win_find_current_thread_info(void);

#define current ((struct task_struct*) win_find_current_thread_info())

#endif /* __ASSEMBLY__ */
#endif
