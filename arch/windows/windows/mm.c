#include <windows/api.h>
#include <wdm.h>

void *win_allocate_memory(unsigned long size)
{
	return ExAllocatePoolWithTag(NonPagedPool, size, 'XINU');
}
