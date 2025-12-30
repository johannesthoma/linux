#ifndef _WINDOWS_SECTION_H
#define _WINDOWS_SECTION_H

/* TODO: get addr passed via DriverEntry and check it here */

static inline int __is_kernel_text(unsigned long addr)
{
	return false;	
}

static inline int __is_kernel(unsigned long addr)
{
	return false;	/* TODO: get addr passed via DriverEntry and check it here */
}

static inline int is_kernel_inittext(unsigned long addr)
{
	return false;
}

static inline int is_kernel_rodata(unsigned long addr)
{
	return false;
}

static inline int init_section_intersects(void *virt, size_t size)
{
	return false;
}

static inline int init_section_contains(void *virt, size_t size)
{
	return false;
}

static inline bool is_kernel_core_data(unsigned long addr)
{
	return false;
}

/* nix */

#endif
