#ifndef _ASM_PAGE_H
#define _ASM_PAGE_H

#include <asm-generic/page.h>
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

#endif
