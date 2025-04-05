struct thread_info {
	int preempt_count;
	unsigned long flags;
	struct task_struct *task;
};

static inline struct thread_info *current_thread_info(void)
{
	return NULL;
}
