#include <linux/init.h>
#include <linux/slab.h>

unsigned long memory_start;
unsigned long memory_end;

void __init mem_init(void)
{
}

void __init pgtable_cache_init(void)
{
}

void *vmalloc_huge(unsigned long size, gfp_t gfp_mask)
{
	return NULL;
}
