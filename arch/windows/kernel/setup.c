#include <linux/init.h>
#include <linux/memblock.h>
#include <windows/api.h>

// #define INIT_MEMBLOCK_SIZE 1024*1024
#define INIT_MEMBLOCK_SIZE 4*1024*1024
// Allocating this fails (at least on ReactOS):
// #define INIT_MEMBLOCK_SIZE 1024*1024*128

void *MmAllocateContiguousMemory(size_t NumberOfBytes, unsigned long long HighestAcceptableAddress);

void __init setup_arch(char **cmdline_p)
{
	unsigned long size;
	int i;

	for (i=0;i<10;i++) {
		size = 128*1024*1024;
		do {
			void *block_of_physical_memory = win_allocate_physical_memory(size);
			if (block_of_physical_memory) {
				printk("%d: Success!!! allocated %d bytes of physical memory.\n", i, size);
				printk("virtual address is %p physical address is 0x%llx\n", block_of_physical_memory, win_virt_to_phys(block_of_physical_memory));
				break;
			} else {
				printk("%d: Failed. Could not allocate %d bytes of physical memory.\n", i, size);
			}
			size /= 2;
		} while (size > 4095);
	}

	void *initial_memory = win_allocate_memory(INIT_MEMBLOCK_SIZE);
	if (initial_memory == NULL) {
		DbgPrint("Oops I don't even have memory for the early booting...\n");
	} else {
		memblock_add((unsigned long) initial_memory, INIT_MEMBLOCK_SIZE);
	}
	/* TODO: read from Windows registry ... */
	// *cmdline_p = "lpj=1000";
	*cmdline_p = "lpj=1000 root=/dev/ram";

	paging_init();
}

