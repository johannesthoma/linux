#ifndef WINDOWSAPI_H
#define WINDOWSAPI_H

struct page;

void *win_allocate_memory(unsigned long size);

void win_add_page(struct page *page);
void win_del_page(struct page *page);

/* page virtual address to struct page */
struct page *win_virt_to_page(const void *vaddr);
void *win_page_to_virt(const struct page *page);

/* timer.c: */
struct win_timer_list;

unsigned long long win_jiffies(void);

struct win_timer_list *win_allocate_timer(void(*callback)(struct win_timer_list *timer));
void win_del_timer(struct win_timer_list *t);
void win_mod_timer_relative(struct win_timer_list *timer, unsigned long long expires);

struct thread_info;
struct task_struct;
struct _KTHREAD;

struct thread_info *win_find_current_thread_info(void);
int win_create_windows_thread(struct task_struct *task, struct _KTHREAD **thread_object_p);

#endif
