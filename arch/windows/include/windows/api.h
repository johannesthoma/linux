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

void win_wake_up_task(struct task_struct *t);
void win_put_task_to_sleep(struct task_struct *t);

#include <linux/spinlock_types.h>

bool win_arch_spin_is_locked(arch_spinlock_t *lock);
bool win_arch_spin_is_contended(arch_spinlock_t *lock);
bool win_arch_spin_is_locked(arch_spinlock_t *lock);
bool win_arch_spin_value_unlocked(arch_spinlock_t *lock);
void win_arch_spin_lock(arch_spinlock_t *lock);
bool win_arch_spin_trylock(arch_spinlock_t *lock);
void win_arch_spin_unlock(arch_spinlock_t *lock);

unsigned long win_spin_lock_irqsave_ret(raw_spinlock_t *lock);
void win_spin_unlock_irqrestore(raw_spinlock_t *lock, unsigned long flags);
void win_spin_lock_irq(raw_spinlock_t *lock);
void win_spin_unlock_irq(raw_spinlock_t *lock);
void win_spin_lock(raw_spinlock_t *lock);
void win_spin_unlock(raw_spinlock_t *lock);
void win_spin_lock_bh(raw_spinlock_t *lock);
void win_spin_unlock_bh(raw_spinlock_t *lock);
void win_spin_lock_nested(raw_spinlock_t *lock, int level);

void win_read_lock(rwlock_t *lock);
void win_read_unlock(rwlock_t *lock);
void win_read_lock_irq(rwlock_t *lock);
void win_read_unlock_irq(rwlock_t *lock);
unsigned long win_read_lock_irqsave_ret(rwlock_t *lock);
void win_read_unlock_irqrestore(rwlock_t *lock, unsigned long flags);
void win_write_lock(rwlock_t *lock);
void win_write_unlock(rwlock_t *lock);
void win_write_lock_bh(rwlock_t *lock);
void win_write_unlock_bh(rwlock_t *lock);
void win_write_lock_irq(rwlock_t *lock);
void win_write_unlock_irq(rwlock_t *lock);
unsigned long win_write_lock_irqsave_ret(rwlock_t *lock);
void win_write_unlock_irqrestore(rwlock_t *lock, unsigned long flags);

void win_disable_preemption(void);
void win_enable_preemption(void);

#endif
