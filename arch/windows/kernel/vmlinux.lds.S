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
	. = 0x10001000;

	_stext = .;
	TEXT_SECTION = 0x10001000;
	.text : AT(0x10001000) { *(.text*) }
	_etext = .;

	__init_begin = .;
	. = ALIGN(PAGE_SIZE);
	HEAD_TEXT_SECTION
	. = ALIGN(PAGE_SIZE);
	INIT_TEXT_SECTION(PAGE_SIZE)
	. = ALIGN(PAGE_SIZE);
	INIT_DATA_SECTION(PAGE_SIZE)
	. = ALIGN(PAGE_SIZE);
	PERCPU_SECTION(PAGE_SIZE)
	. = ALIGN(PAGE_SIZE);
	.idata : { *(.idata*) }
	__init_end = .;

	_sdata = .;
	. = ALIGN(PAGE_SIZE);
	RO_DATA(PAGE_SIZE)
	. = ALIGN(PAGE_SIZE);
	RW_DATA(PAGE_SIZE, PAGE_SIZE, THREAD_SIZE)
	_edata = .;

	EXCEPTION_TABLE(PAGE_SIZE)

	BSS_SECTION(0, 0, 0)
	_end = .;

//	STABS_DEBUG
//	DWARF_DEBUG
//	ELF_DETAILS

//	DISCARDS		// must be the last
}

// SEARCH_DIR(/home/johannes/.zeranoe/mingw-w64/i686/i686-w64-mingw32/lib)
// INPUT(-lntoskrnl -lhal -lntdll -lnetio)
