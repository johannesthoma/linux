#include <linux/spinlock_types.h>

/* Those are defined in linux/types.h */
#define _UINTPTR_T_DEFINED 1
#define _INTPTR_T_DEFINED 1

#include <ntdef.h>
#include <ntddk.h>

bool win_arch_spin_is_locked(arch_spinlock_t *lock)
{
	return lock->windows_lock != 0;
}

bool win_arch_spin_is_contended(arch_spinlock_t *lock)
{
	return lock->windows_lock > 10000;	/* TODO: or so */
}

bool win_arch_spin_value_unlocked(arch_spinlock_t *lock)
{
	return lock->windows_lock == 0;
}

void win_arch_spin_lock(arch_spinlock_t *lock)
{
	KeAcquireSpinLockAtDpcLevel(&lock->windows_lock);
}

bool win_arch_spin_trylock(arch_spinlock_t *lock)
{	/* TODO: try ... */
	KeAcquireSpinLockAtDpcLevel(&lock->windows_lock);
	return true;
}

void win_arch_spin_unlock(arch_spinlock_t *lock)
{
	KeReleaseSpinLockFromDpcLevel(&lock->windows_lock);
}

unsigned long win_spin_lock_irqsave_ret(spinlock_t *lock)
{
	KIRQL old_irql;

	KeAcquireSpinLock(&lock->rlock.raw_lock.windows_lock, &old_irql);
	return old_irql;
}

void win_spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags)
{
	KeReleaseSpinLock(&lock->rlock.raw_lock.windows_lock, (KIRQL) flags);
}

void win_spin_lock_irq(spinlock_t *lock)
{
	KIRQL ignored;

	KeAcquireSpinLock(&lock->rlock.raw_lock.windows_lock, &ignored);
}

void win_spin_unlock_irq(spinlock_t *lock)
{
	KeReleaseSpinLock(&lock->rlock.raw_lock.windows_lock, PASSIVE_LEVEL);
}

void win_spin_lock(spinlock_t *lock)
{
	KeAcquireSpinLockAtDpcLevel(&lock->rlock.raw_lock.windows_lock);
}

void win_spin_unlock(spinlock_t *lock)
{
	KeReleaseSpinLockFromDpcLevel(&lock->rlock.raw_lock.windows_lock);
}

void win_spin_lock_bh(spinlock_t *lock)
{
	KeAcquireSpinLockAtDpcLevel(&lock->rlock.raw_lock.windows_lock);
}

void win_spin_unlock_bh(spinlock_t *lock)
{
	KeReleaseSpinLockFromDpcLevel(&lock->rlock.raw_lock.windows_lock);
}

void win_spin_lock_nested(spinlock_t *lock, int level)
{
	/* TODO: level? */
	KeAcquireSpinLockAtDpcLevel(&lock->rlock.raw_lock.windows_lock);
}

#ifndef CONFIG_HAVE_RW_LOCKS

/* And now, the rw_locks. Not supported before NT6 (Vista) so
 * for ancient Windows this maps to spin_lock's here.
 */

void win_read_lock(rwlock_t *lock)
{
	win_spin_lock((spinlock_t*) lock);
}

void win_read_unlock(rwlock_t *lock)
{
	win_spin_unlock((spinlock_t*) lock);
}

void win_read_lock_irq(rwlock_t *lock)
{
	win_spin_lock_irq((spinlock_t*) lock);
}

void win_read_unlock_irq(rwlock_t *lock)
{
	win_spin_unlock_irq((spinlock_t*) lock);
}

unsigned long win_read_lock_irqsave_ret(rwlock_t *lock)
{
	KIRQL flags;

	flags = win_spin_lock_irqsave_ret((spinlock_t*) lock);
	return flags;
}

void win_read_unlock_irqrestore(rwlock_t *lock, unsigned long flags)
{
	win_spin_unlock_irqrestore((spinlock_t*) lock, flags);
}

void win_write_lock(rwlock_t *lock)
{
	win_spin_lock((spinlock_t*) lock);
}

void win_write_unlock(rwlock_t *lock)
{
	win_spin_unlock((spinlock_t*) lock);
}

void win_write_lock_irq(rwlock_t *lock)
{
	win_spin_lock_irq((spinlock_t*) lock);
}

void win_write_unlock_irq(rwlock_t *lock)
{
	win_spin_unlock_irq((spinlock_t*) lock);
}

unsigned long win_write_lock_irqsave_ret(rwlock_t *lock)
{
	KIRQL flags;

	flags = win_spin_lock_irqsave_ret((spinlock_t*) lock);
	return flags;
}

void win_write_unlock_irqrestore(rwlock_t *lock, unsigned long flags)
{
	win_spin_unlock_irqrestore((spinlock_t*) lock, flags);
}

#else

/* And now, the rw_locks using ExAcquireSpinLockShared and friends.
 * No recursion detection here. Also no DPC checking. It is the
 * same as the Linux implementation (I think :) ).
 */

void win_read_lock(rwlock_t *lock)
{
	ExAcquireSpinLockSharedAtDpcLevel(&lock->raw_lock.windows_lock);
}

void win_read_unlock(rwlock_t *lock)
{
	ExReleaseSpinLockSharedFromDpcLevel(&lock->raw_lock.windows_lock);
}

void win_read_lock_irq(rwlock_t *lock)
{
	unsigned long ignored;

	ignored = ExAcquireSpinLockShared(&lock->raw_lock.windows_lock);
}

void win_read_unlock_irq(rwlock_t *lock)
{
	ExReleaseSpinLockShared(&lock->raw_lock.windows_lock, PASSIVE_LEVEL);
}

unsigned long win_read_lock_irqsave_ret(rwlock_t *lock)
{
	return ExAcquireSpinLockShared(&lock->raw_lock.windows_lock);
}

void win_read_unlock_irqrestore(rwlock_t *lock, unsigned long flags)
{
	ExReleaseSpinLockShared(&lock->raw_lock.windows_lock, flags);
}

void win_write_lock(rwlock_t *lock)
{
	ExAcquireSpinLockExclusiveAtDpcLevel(&lock->raw_lock.windows_lock);
}

void win_write_unlock(rwlock_t *lock)
{
	ExReleaseSpinLockExclusiveFromDpcLevel(&lock->raw_lock.windows_lock);
}

void win_write_lock_bh(rwlock_t *lock)
{
	ExAcquireSpinLockExclusiveAtDpcLevel(&lock->raw_lock.windows_lock);
}

void win_write_unlock_bh(rwlock_t *lock)
{
	ExReleaseSpinLockExclusiveFromDpcLevel(&lock->raw_lock.windows_lock);
}

void win_write_lock_irq(rwlock_t *lock)
{
	unsigned long ignored;

	ignored = ExAcquireSpinLockExclusive(&lock->raw_lock.windows_lock);
}

void win_write_unlock_irq(rwlock_t *lock)
{
	ExReleaseSpinLockExclusive(&lock->raw_lock.windows_lock, guess_old_kirql());
}

unsigned long win_write_lock_irqsave_ret(rwlock_t *lock)
{
	return ExAcquireSpinLockExclusive(&lock->raw_lock.windows_lock);
}

void win_write_unlock_irqrestore(rwlock_t *lock, unsigned long flags)
{
	ExReleaseSpinLockExclusive(&lock->raw_lock.windows_lock, flags);
}

#endif
