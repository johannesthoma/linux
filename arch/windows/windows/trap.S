#include <linux/linkage.h>
#include <linux/export.h>

        .section .entry.text, "ax"

	.global _fault_handler_mux
_fault_handler_mux:
	jmp *_original_fault_handler(,1)


