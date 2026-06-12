#ifndef WINDOWSAPI_H
#define WINDOWSAPI_H

struct page;

void *win_allocate_memory(unsigned long size);

/* page virtual address to struct page */
struct page *win_virt_to_page(const void *vaddr);
void *win_page_to_virt(const struct page *page);

struct page *win_alloc_pages(int gfp, unsigned int order);

struct win_timer_list;

struct win_timer_list *win_allocate_timer(void(*callback)(struct win_timer_list *timer));
void win_del_timer(struct win_timer_list *t);
void win_mod_timer_relative(struct win_timer_list *timer, unsigned long long expires);

struct thread_info;
struct task_struct;
struct _KTHREAD;

struct thread_info *win_find_current_thread_info(void);
int win_create_windows_thread(struct task_struct *task, struct _KTHREAD **thread_object_p);

void win_wake_up_task(struct task_struct *t);
void win_put_task_to_sleep(struct task_struct *t);

void win_disable_preemption(void);
void win_enable_preemption(void);

void win_setup_int80(void (*fn)(void));

#endif
