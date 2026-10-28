#include <linux/linkage.h>
#include <linux/export.h>

# TODO: needed?
#        .section .entry.text, "ax"

	.global _fault_handler_mux
_fault_handler_mux:
	push %eax
	movl %cr3, %eax
	addl $0x8000000, %eax
	cmpl $0x42420000, 0xe84(%eax)
	je 1f
	pop %eax
	jmp *_original_fault_handler(,1)
1:
	pop %eax
	jmp asm_exc_page_fault


