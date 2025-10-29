#include <linux/init.h>
#include <linux/slab.h>
#include <linux/hashtable.h>
#include <linux/mm.h>

unsigned long memory_start;
unsigned long memory_end;

/* page va to struct page */

/* TODO: hash table */

struct page *win_virt_to_page(const void *vaddr)
{
	return NULL;	/* TODO: look up in hash table */
}

void *win_page_to_virt(const struct page *page)
{
	return page_address(page);
}

void __init mem_init(void)
{
/* TODO: init hash table */
}

void __init pgtable_cache_init(void)
{
}

void *vmalloc_huge(unsigned long size, gfp_t gfp_mask)
{
	return NULL;
}
