#ifndef _ASM_PAGE_H
#define _ASM_PAGE_H

/* PAGE_SHIFT determines the page size */

#define PAGE_SHIFT	12
#ifdef __ASSEMBLY__
#define PAGE_SIZE	(1 << PAGE_SHIFT)
#else
#define PAGE_SIZE	(1UL << PAGE_SHIFT)
#endif
#define PAGE_MASK	(~(PAGE_SIZE-1))

#include <asm/setup.h>

#ifndef __ASSEMBLY__

#define clear_page(page)	memset((page), 0, PAGE_SIZE)
#define copy_page(to,from)	memcpy((to), (from), PAGE_SIZE)

#define clear_user_page(page, vaddr, pg)	clear_page(page)
#define copy_user_page(to, from, vaddr, pg)	copy_page(to, from)

/*
 * These are used to make use of C type-checking..
 */
typedef struct {
	unsigned long pte;
} pte_t;
typedef struct {
	unsigned long pmd[16];
} pmd_t;
typedef struct {
	unsigned long pgd;
} pgd_t;
typedef struct {
	unsigned long pgprot;
} pgprot_t;
typedef struct page *pgtable_t;

#define pte_val(x)	((x).pte)
#define pmd_val(x)	((&x)->pmd[0])
#define pgd_val(x)	((x).pgd)
#define pgprot_val(x)	((x).pgprot)

#define __pte(x)	((pte_t) { (x) } )
#define __pmd(x)	((pmd_t) { (x) } )
#define __pgd(x)	((pgd_t) { (x) } )
#define __pgprot(x)	((pgprot_t) { (x) } )

extern unsigned long memory_start;
extern unsigned long memory_end;

#endif /* !__ASSEMBLY__ */

#define PAGE_OFFSET		(0)

#ifndef ARCH_PFN_OFFSET
#define ARCH_PFN_OFFSET		(PAGE_OFFSET >> PAGE_SHIFT)
#endif

#ifndef __ASSEMBLY__

#define __va(x) ((void *)((unsigned long) (x)))
#define __pa(x) ((unsigned long) (x))

static inline unsigned long virt_to_pfn(const void *kaddr)
{
	return __pa(kaddr) >> PAGE_SHIFT;
}
#define virt_to_pfn virt_to_pfn
static inline void *pfn_to_virt(unsigned long pfn)
{
	return __va(pfn << PAGE_SHIFT);
}
#define pfn_to_virt pfn_to_virt

#define virt_to_page(addr)	win_virt_to_page(addr)
#define page_to_virt(page)	win_page_to_virt(page)

#ifndef page_to_phys
#define page_to_phys(page)      ((dma_addr_t)page_to_pfn(page) << PAGE_SHIFT)
#endif

#define	virt_addr_valid(kaddr)	(((void *)(kaddr) >= (void *)PAGE_OFFSET) && \
				((void *)(kaddr) < (void *)memory_end))

#endif /* __ASSEMBLY__ */

#include <asm-generic/memory_model.h>
#include <asm-generic/getorder.h>

#include <asm-generic/pgtable-nopud.h>

/* TODO: -> thread_info.h ? */
#define THREAD_SIZE_ORDER 1
#define THREAD_SIZE PAGE_SIZE
#define TASK_SIZE 0x80000000

/* No pages, they are maintained by the Windows kernel */
#define PAGE_KERNEL __pgprot(0)

/* This adds the virtual member to struct page. It is needed
 * to store the actual virtual address returned by ExAllocatePool().
 */
#define WANT_PAGE_VIRTUAL 1

#ifndef __ASSEMBLY__
struct page *win_virt_to_page(const void *vaddr);
void *win_page_to_virt(const struct page *page);
#endif

#endif
