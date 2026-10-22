#include "internal.h"

/* See ntoskrnl/ke/i386/cpu.c in the ReactOS kernel */

void win_setup_int80(void (*fn)(void))
{
	PKIDTENTRY IdtEntry;

    /* Get the IDT Entry for Interrupt 0x80 */
	IdtEntry = &((PKIPCR)KeGetPcr())->IDT[0x80];

    /* Set it up */
	IdtEntry->Selector = KGDT_R0_CODE;
	IdtEntry->Offset = ((ULONG_PTR)fn & 0xFFFF);
	IdtEntry->ExtendedOffset = ((ULONG_PTR)fn >> 16) & 0xFFFF;

	((PKIDT_ACCESS)&IdtEntry->Access)->Dpl = 3;
	((PKIDT_ACCESS)&IdtEntry->Access)->Present = 1;
	((PKIDT_ACCESS)&IdtEntry->Access)->SegmentType = I386_INTERRUPT_GATE;
}

// void (*original_fault_handler)(void);
ULONG_PTR original_fault_handler;

	/* Hooks into the memory fault handler. Used to dispatch
         * between address spaces owned by Windows and ones owned
         * by Linux. Must be called only once.
         */

void win_intercept_memory_fault_handler(void (*fn)(void))
{
	PKIDTENTRY IdtEntry;

    /* Get the IDT Entry for Interrupt 0x0E, which is the Page-Fault
     * Exception (#PF) */
	IdtEntry = &((PKIPCR)KeGetPcr())->IDT[0x0E];

    /* Remember the original handler address */
	original_fault_handler = (IdtEntry->ExtendedOffset << 16) |  IdtEntry->Offset;

    /* just alter the handler's address */
	IdtEntry->Offset = ((ULONG_PTR)fn & 0xFFFF);
	IdtEntry->ExtendedOffset = ((ULONG_PTR)fn >> 16) & 0xFFFF;
}
