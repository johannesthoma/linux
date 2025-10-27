#include <asm-generic/vmlinux.lds.h>
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
	. = 0x40000000;
	__init_begin = .;
	HEAD_TEXT_SECTION
	INIT_TEXT_SECTION(PAGE_SIZE)
	INIT_DATA_SECTION(16)
	PERCPU_SECTION(CACHELINE_SIZE)
	__init_end = .;

	_stext = .;
	TEXT_SECTION = 0;
	_etext = .;

	_sdata = .;
	RO_DATA(PAGE_SIZE)
	RW_DATA(16, PAGE_SIZE, THREAD_SIZE)
	_edata = .;

	EXCEPTION_TABLE(16)

	BSS_SECTION(0, 0, 0)
	_end = .;

	STABS_DEBUG
	DWARF_DEBUG
	ELF_DETAILS

	DISCARDS		// must be the last
}
