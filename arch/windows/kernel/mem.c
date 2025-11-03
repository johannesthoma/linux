#include <linux/init.h>
#include <linux/slab.h>
#include <linux/hashtable.h>
#include <linux/mm.h>

unsigned long memory_start;
unsigned long memory_end;

/* Since both struct page and the virtual address of the page
 * are obtained by ExAllocatePool() (which returns unpredictable
 * values) we need something to optimize virt_to_page queries.
 * Therefore the hash table (indexed by hash of the virtual
 * address) is used here.
 */

DEFINE_HASHTABLE(virt_to_page_hashtable, 11);

void win_add_page(struct page *page)
{
	hash_add(virt_to_page_hashtable, &page->hlist, (unsigned long) page->virtual);
}

void win_del_page(struct page *page)
{
	hash_del(&page->hlist);
}

/* page virtual address to struct page */
struct page *win_virt_to_page(const void *vaddr)
{
	struct page *page;

	hash_for_each_possible(virt_to_page_hashtable, page, hlist, (unsigned long) vaddr) {
		if (page->virtual == vaddr)
			return page;
	}
		/* TODO: what now? */
	return NULL;
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
