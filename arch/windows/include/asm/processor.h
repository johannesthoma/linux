#ifndef WINDOWS_PROCESSOR_H
#define WINDOWS_PROCESSOR_H

#define cpu_relax() do { } while (0)

struct thread_struct {
	unsigned long nix;
};

#define INIT_THREAD  {  }

static inline unsigned long __get_wchan(struct task_struct *p)
{
	return 0;
}

#endif
