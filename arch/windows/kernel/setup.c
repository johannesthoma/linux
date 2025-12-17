#include <linux/init.h>
#include <linux/memblock.h>
#include <windows/api.h>

#define INIT_MEMBLOCK_SIZE 1024*1024

void __init setup_arch(char **cmdline_p)
{
	void *initial_memory = win_allocate_memory(INIT_MEMBLOCK_SIZE);
	if (initial_memory == NULL) {
		DbgPrint("Oops I don't even have memory for the early booting...\n");
	} else {
		memblock_add((unsigned long) initial_memory, INIT_MEMBLOCK_SIZE);
	}
	/* TODO: read from Windows registry ... */
	*cmdline_p = "lpj=1000";
}

