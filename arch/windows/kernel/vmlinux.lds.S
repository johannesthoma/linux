// not asm-generic!
#include <asm/vmlinux.lds.h>
#include <asm/page.h>

// OUTPUT_FORMAT(pe)
#ifdef i386
#undef i386
#endif

#ifndef CACHELINE_SIZE
#define CACHELINE_SIZE 16
#endif

OUTPUT_ARCH(i386)
ENTRY(_DriverEntry)
SECTIONS
{
	. = 0x10001000;

	__stext = .;
	TEXT_SECTION = 0x10001000;
	.text : AT(0x10001000) {
		*(.text*)
		TEXT_TEXT
		SCHED_TEXT
	}
	__etext = .;

	___init_begin = .;
	. = ALIGN(PAGE_SIZE);
	HEAD_TEXT_SECTION
	. = ALIGN(PAGE_SIZE);
	INIT_TEXT_SECTION(PAGE_SIZE)
	. = ALIGN(PAGE_SIZE);
	INIT_DATA_SECTION(PAGE_SIZE)
	. = ALIGN(PAGE_SIZE);
	PERCPU_SECTION(PAGE_SIZE)
	___init_end = .;

  .idata BLOCK(__section_alignment__) :
  {
    /* This cannot currently be handled with grouped sections.
        See pe.em:sort_sections.  */
    KEEP (SORT(*)(.idata$2))
    KEEP (SORT(*)(.idata$3))
    /* These zeroes mark the end of the import list.  */
    LONG (0); LONG (0); LONG (0); LONG (0); LONG (0);
    KEEP (SORT(*)(.idata$4))
    ___IAT_start__ = .;
    KEEP (SORT(*)(.idata$5))
    ___IAT_end__ = .;
    KEEP (SORT(*)(.idata$6))
    KEEP (SORT(*)(.idata$7))
  }

	__sdata = .;
	. = ALIGN(PAGE_SIZE);
	RO_DATA(PAGE_SIZE)
	. = ALIGN(PAGE_SIZE);
	RW_DATA(PAGE_SIZE, PAGE_SIZE, THREAD_SIZE)
	__edata = .;

	EXCEPTION_TABLE(PAGE_SIZE)

	BSS_SECTION(0, 0, 0)
	_end = .;

	STABS_DEBUG
	DWARF_DEBUG
	ELF_DETAILS

	DISCARDS		// must be the last */
}

// SEARCH_DIR(/home/johannes/.zeranoe/mingw-w64/i686/i686-w64-mingw32/lib)
// INPUT(-lntoskrnl -lhal -lntdll -lnetio)


