/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_SPINLOCK_TYPES_H
#define _ASM_SPINLOCK_TYPES_H

lebt

#ifndef __LINUX_SPINLOCK_TYPES_RAW_H
# error "please don't include this file directly"
#endif

typedef struct {
		/* This should be a KSPIN_LOCK */
	unsigned long windows_lock;
} arch_spinlock_t;

#define __ARCH_SPIN_LOCK_UNLOCKED	{ 0 }

typedef struct {
		/* This is a EX_SPIN_LOCK (but only on x86_64) */
	signed int windows_lock;
} arch_rwlock_t;

#define __ARCH_RW_LOCK_UNLOCKED		{ 0 }

#endif /* _ASM_SPINLOCK_TYPES_H */
