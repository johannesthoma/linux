#include <linux/init.h>
#include <linux/slab.h>
#include <linux/hashtable.h>
#include <linux/mm.h>
#include <linux/memblock.h>
#include <windows/api.h>

unsigned long memory_start;
unsigned long memory_end;

/* 128 MB for this Linux driver */
#define NR_PAGES_AVAILABLE (128*1024*1024/PAGE_SIZE)

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

	vaddr = (const void *)((unsigned long) vaddr & PAGE_MASK);
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

void __init paging_init(void)
{
	unsigned long max_zone_pfn[MAX_NR_ZONES] = { 0 };

	max_zone_pfn[ZONE_NORMAL] = NR_PAGES_AVAILABLE;
	free_area_init(max_zone_pfn);
}

void __init mem_init(void)
{
		/* This should 'release the pages to the buddy
		 * allocator'
		 */

#if 0
	memblock_free_all();
#endif
		/* fake 128 MB RAM for now ... */
	totalram_pages_add(128*1024*1024 / PAGE_SIZE);
}

void __init pgtable_cache_init(void)
{
}

void *vmalloc_huge(unsigned long size, gfp_t gfp_mask)
{
	return win_allocate_memory(size);
}

extern void prep_compound_page(struct page *page, unsigned int order);

struct page *win_alloc_pages(int gfp, unsigned int order)
{
	int i;
	struct page *pages;
	void *mem = win_allocate_memory(PAGE_SIZE << order);

	if (mem == NULL)
		return NULL;

	pages = win_allocate_memory(sizeof(pages[0]) * (1 << order));
	if (pages == NULL) {
		/* TODO: free mem */
		return NULL;
	}
	memset(pages, 0, sizeof(pages[0]) * (1 << order));

	for (i = 0; i < (1 << order); i++) {
		set_page_address(&pages[i], mem + PAGE_SIZE*i);
		win_add_page(&pages[i]);
	}

		/* see mm/page_alloc.c */
	if (order && (gfp & __GFP_COMP))
		prep_compound_page(pages, order);

	return pages;
}
