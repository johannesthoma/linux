#include <ntdef.h>
#include <ntddk.h>

// typedef int NTSTATUS;
// #define STATUS_SUCCESS 0

// extern int DbgPrint(const char *fmt, ...);

// NTSTATUS __attribute__((stdcall)) DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING registry_path)

extern void start_kernel(void);

NTSTATUS __attribute__((stdcall)) DriverEntry(void *DriverObject, void *registry_path)
{
	KIRQL irql;

	DbgPrint("Hallo Linux!!\n");

		/* Linux expects interrupts to be disabled here.
		 * It uses spin_lock() which expects irql at
		 * DISPATCH_LEVEL (or higher).
		 */
//	KeRaiseIrql(DISPATCH_LEVEL, &irql);
	start_kernel();
//	KeLowerIrql(irql);

	return STATUS_SUCCESS;
}

