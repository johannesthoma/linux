#include <windows/api.h>
#include <wdm.h>

void *win_allocate_memory(unsigned long size)
{
	if (size == 0)
		size = 1;	/* return something */

	return ExAllocatePool(NonPagedPool, size);
}
