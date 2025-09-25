#ifndef _ASM_PAGE_H
#define _ASM_PAGE_H

#include <asm-generic/page.h>
#include <asm-generic/pgtable-nopud.h>

#define THREAD_SIZE PAGE_SIZE
#define TASK_SIZE 0x80000000

/* No pages, they are maintained by the Windows kernel */
#define PAGE_KERNEL (void*)0

#if 0
#define PAGE_SIZE 4096
#define PAGE_MASK ~(PAGE_SIZE-1)
#define PAGE_SHIFT 12

/* taken from arm/include/asm/page_nommu.h */
#define clear_page(page)	memset((page), 0, PAGE_SIZE)
#define copy_page(to,from)	memcpy((to), (from), PAGE_SIZE)

#define clear_user_page(page, vaddr, pg)	clear_page(page)
#define copy_user_page(to, from, vaddr, pg)	copy_page(to, from)

typedef struct page *pgtable_t;

/*
 * These are used to make use of C type-checking..
 */
typedef unsigned long pte_t;
typedef unsigned long pmd_t;
typedef unsigned long pgd_t[2];
typedef unsigned long pgprot_t;

#define pte_val(x)      (x)
#define pmd_val(x)      (x)
#define pgd_val(x)	((x)[0])
#define pgprot_val(x)   (x)

#define __pte(x)        (x)
#define __pmd(x)        (x)
#define __pgprot(x)     (x)

#include <asm-generic/pgtable-nopud.h>

#define virt_to_page(addr) (struct page*)(0)
#define page_to_phys(p) (0)
#define page_to_pfn(addr) (0)
#define pfn_to_page(addr) (struct page*)(0)
#define __va(phys) (0)
#define __pa(virt) (0)

#endif

#endif
