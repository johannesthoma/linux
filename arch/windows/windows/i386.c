/*
#include "internal.h"
#include <ketypes.h>
*/
#include "ntoskrnl.h"

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

