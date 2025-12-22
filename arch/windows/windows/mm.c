#include <windows/api.h>

#define _INTPTR_T_DEFINED
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
