#ifndef _WINDOWS_PGTABLE_H
#define _WINDOWS_PGTABLE_H

#define ZERO_PAGE(x) 0
#define VMALLOC_START 0
#define VMALLOC_END 0x0fffffff

#define PMD_SHIFT  21

#define swapper_pg_dir ((pgd_t *) 0)

#endif
