#ifndef __ARCH_WINDOWS_SPINLOCK_H
#define __ARCH_WINDOWS_SPINLOCK_H

#include <windows/api.h>

zak

#define in_lock_functions(ADDR)         0
#define assert_raw_spin_locked(lock)    do { (void)(lock); } while (0)

#define arch_spin_is_locked(l)          win_arch_spin_is_locked(l)
#define arch_spin_is_contended(l)       win_arch_spin_is_contended(l)
#define arch_spin_value_unlocked(l)     win_arch_spin_value_unlocked(l)
#define arch_spin_lock(l)               win_arch_spin_lock(l)
#define arch_spin_trylock(l)            win_arch_spin_trylock(l)
#define arch_spin_unlock(l)             win_arch_spin_unlock(l)

#define _raw_spin_lock(lock)			win_spin_lock(lock)
#define _raw_spin_lock_nested(lock, subclass)	win_spin_lock(lock)
#define _raw_read_lock(lock)			win_read_lock(lock)
#define _raw_write_lock(lock)			win_write_lock(lock)
#define _raw_write_lock_nested(lock, subclass)	win_write_lock(lock)
#define _raw_spin_lock_bh(lock)			win_spin_lock_bh(lock)
#define _raw_read_lock_bh(lock)			win_read_lock_bh(lock)
#define _raw_write_lock_bh(lock)		win_write_lock_bh(lock)
#define _raw_spin_lock_irq(lock)		win_spin_lock_irq(lock)
#define _raw_read_lock_irq(lock)		win_read_lock_irq(lock)
#define _raw_write_lock_irq(lock)		win_write_lock_irq(lock)
#define _raw_spin_lock_irqsave(lock, flags)	flags = win_spin_lock_irqsave_ret(lock)
#define _raw_read_lock_irqsave(lock, flags)	flags = win_read_lock_irqsave_ret(lock)
#define _raw_write_lock_irqsave(lock, flags)	flags = win_write_lock_irqsave_ret(lock)
#define _raw_spin_trylock(lock)			({ win_spin_lock(lock); 1; })
#define _raw_read_trylock(lock)			({ win_read_lock(lock); 1; })
#define _raw_write_trylock(lock)			({ win_write_lock(lock); 1; })
#define _raw_spin_trylock_bh(lock)		({ win_spin_lock(lock); 1; })
#define _raw_spin_unlock(lock)			win_spin_unlock(lock)
#define _raw_read_unlock(lock)			win_read_unlock(lock)
#define _raw_write_unlock(lock)			win_write_unlock(lock)
#define _raw_spin_unlock_bh(lock)		win_spin_unlock_bh(lock)
#define _raw_read_unlock_bh(lock)		win_read_unlock_bh(lock)
#define _raw_write_unlock_bh(lock)		win_write_unlock_bh(lock)
#define _raw_spin_unlock_irq(lock)		win_spin_unlock_irq(lock)
#define _raw_read_unlock_irq(lock)		win_read_unlock_irq(lock)
#define _raw_write_unlock_irq(lock)		win_write_unlock_irq(lock)
#define _raw_spin_unlock_irqrestore(lock, flags) \
					win_spin_unlock_irqrestore(lock, flags)
#define _raw_read_unlock_irqrestore(lock, flags) \
					win_read_unlock_irqrestore(lock, flags)
#define _raw_write_unlock_irqrestore(lock, flags) \
					win_write_unlock_irqrestore(lock, flags)

#endif
