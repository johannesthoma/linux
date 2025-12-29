#include <windows/api.h>

/* Those are defined in linux/types.h */
#define _UINTPTR_T_DEFINED 1
#define _INTPTR_T_DEFINED 1

#include <wdm.h>

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

