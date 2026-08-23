#include <windows/api.h>
#include <linux/types.h>

#include "internal.h"

#if 0
/* Those are defined in linux/types.h */
#define _UINTPTR_T_DEFINED 1
#define _INTPTR_T_DEFINED 1

#include <wdm.h>
#endif

void *win_allocate_memory(unsigned long size)
{
	if (size == 0)
		size = 1;	/* return something */

	return ExAllocatePool(NonPagedPool, size);
}

	/* Useful for calling functions in gdb. */
void *malloc(size_t size)
{
	return win_allocate_memory(size);
}

void *win_allocate_physical_memory(unsigned long size)
{
	LARGE_INTEGER l;
	l.QuadPart = (unsigned long long) -1;

	if (size == 0)
		size = 1;	/* return something */

	return MmAllocateContiguousMemory(size, l);
}

unsigned long long win_virt_to_phys(void *virt)
{
	LARGE_INTEGER p = MmGetPhysicalAddress(virt);
	return p.QuadPart;
}
