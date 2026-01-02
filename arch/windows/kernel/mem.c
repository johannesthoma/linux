#include <linux/init.h>
#include <linux/slab.h>
#include <linux/hashtable.h>
#include <linux/mm.h>
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
}

void __init pgtable_cache_init(void)
{
}

void *vmalloc_huge(unsigned long size, gfp_t gfp_mask)
{
	return win_allocate_memory(size);
}

/* Those 3 functions are taken from mm/internal.h: */

static void win_folio_set_order(struct folio *folio, unsigned int order)
{
        if (WARN_ON_ONCE(!order || !folio_test_large(folio)))
                return;

        folio->_flags_1 = (folio->_flags_1 & ~0xffUL) | order;
#ifdef CONFIG_64BIT
        folio->_folio_nr_pages = 1U << order;
#endif
}

static void win_prep_compound_head(struct page *page, unsigned int order)
{
        struct folio *folio = (struct folio *)page;

        win_folio_set_order(folio, order);
        atomic_set(&folio->_entire_mapcount, -1);
        atomic_set(&folio->_nr_pages_mapped, 0);
        atomic_set(&folio->_pincount, 0);
}

static void win_prep_compound_tail(struct page *p, struct page *head, int tail_idx)
{
        p->mapping = TAIL_MAPPING;
        set_compound_head(p, head);
        set_page_private(p, 0);
}

struct page *win_alloc_pages(int gfp, unsigned int order)
{
	int i;
	struct page *page;
	struct page *first_page = NULL;

	void *mem = win_allocate_memory(PAGE_SIZE << order);
	if (mem == NULL)
		return NULL;

	for (i=0;i<(1<<order);i++) {
		if (first_page == NULL && order > 0)
			page = win_allocate_memory(sizeof(struct folio));
		else 
			page = win_allocate_memory(sizeof(*page));

		if (page == NULL) {
			/* TODO: free again */
			return NULL;
		}
		memset(page, 0, sizeof(*page));
		if (first_page == NULL) {
			first_page = page;
		        __SetPageHead(page);
		} else {
			win_prep_compound_tail(page, first_page, order);
		}

		set_page_address(page, mem);
		win_add_page(page);

		mem += PAGE_SIZE;
	}
	if (order > 0)
		win_prep_compound_head(first_page, order);

	return first_page;
}
