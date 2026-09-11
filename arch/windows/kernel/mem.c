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

unsigned long __aligned(PAGE_SIZE) empty_zero_page[PAGE_SIZE / sizeof(unsigned long)] = { 0 };

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

/*
void *vmalloc_huge(unsigned long size, gfp_t gfp_mask)
{
	return win_allocate_memory(size);
}
*/

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

void free_initmem(void)
{
	printk("Would free initmem now, but this is not supported on Windows.\n");
}

void arch_sync_kernel_mappings(unsigned long start, unsigned long end)
{
	printk("arch_sync_kernel_mappings(%ld, %ld) unimplemented!\n", start, end);
}

#define DEFAULT_PTE_MASK ~(_PAGE_NX | _PAGE_GLOBAL)

/* Bits supported by the hardware: */
pteval_t __supported_pte_mask __read_mostly = DEFAULT_PTE_MASK;
/* Bits allowed in normal kernel mappings: */
pteval_t __default_kernel_pte_mask __read_mostly = DEFAULT_PTE_MASK;
EXPORT_SYMBOL_GPL(__supported_pte_mask);
/* Used in PAGE_KERNEL_* macros which are reasonably used out-of-tree: */
EXPORT_SYMBOL(__default_kernel_pte_mask);


void flush_tlb_kernel_range(unsigned long start, unsigned long end)
{
	printk("flush_tlb_kernel_range(%ld, %ld) unimplemented!\n", start, end);
}

void flush_tlb_mm_range(struct mm_struct *mm, unsigned long start,
                                unsigned long end, unsigned int stride_shift,
                                bool freed_tables)
{       
	printk("flush_tlb_mm_range (%ld, %ld) unimplemented!\n", start, end);
}

void __iomem *ioremap(phys_addr_t offset, size_t size)
{
	printk("ioremap offset: %d size: %zd not implemented\n", offset, size);
	return NULL;
}

void iounmap(volatile void __iomem *addr)
{
	printk("iounmap addr: %p is not implemented\n", addr);
}

pgd_t *pgd_alloc(struct mm_struct *mm)
{
	printk("pgd_alloc %p not implemented\n", mm);
	return NULL;
}

void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	printk("pgd_free %p %p not implemented\n", mm, pgd);
}

pgtable_t pte_alloc_one(struct mm_struct *mm)
{
	printk("pte_alloc_one %p not implemented\n", mm);
	return 0;
}

void ___pte_free_tlb(struct mmu_gather *tlb, struct page *pte)
{
	printk("___pte_free_tlb not implemented\n");
}

pte_t pte_mkwrite(pte_t pte, struct vm_area_struct *vma)
{
	pte_t p = { 0 };

	printk("pte_mkwrite not implemented\n");
	return p;
}

int ptep_clear_flush_young(struct vm_area_struct *vma,
                           unsigned long address, pte_t *ptep)
{
	printk("ptep_clear_flush_young not implemented\n");
	return 0;
}

int ptep_set_access_flags(struct vm_area_struct *vma,
                          unsigned long address, pte_t *ptep,
                          pte_t entry, int dirty)
{
	printk("ptep_set_access_flags not implemented\n");
	return 0;
}

int ptep_test_and_clear_young(struct vm_area_struct *vma,
                              unsigned long addr, pte_t *ptep)
{
	printk("ptep_test_and_clear_young not implemented\n");
	return 0;
}

pgd_t swapper_pg_dir[1024];
