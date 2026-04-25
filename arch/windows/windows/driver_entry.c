#include <linux/panic.h>

/* Those are defined in linux/types.h */
#define _UINTPTR_T_DEFINED 1
#define _INTPTR_T_DEFINED 1

#include <ntdef.h>
#include <ntddk.h>

void __attribute__((stdcall)) unload_driver(IN PDRIVER_OBJECT DriverObject)
{
	panic("unload_driver not implemented\n");
}

extern void start_kernel(void);

PDRIVER_OBJECT driver_object;

NTSTATUS __attribute__((stdcall)) DriverEntry(PDRIVER_OBJECT driver_object_param, PUNICODE_STRING registry_path_param)
{
	DbgPrint("Hallo Linux!!\n");

	driver_object = driver_object_param;
	/* TODO: init_registry(registry_path_param) here? */

	start_kernel();

	/* TODO: if we call KiSwapContext() from switch_context() we
	 * would need the idle thread, in which case the start_kernel()
	 * never returns ...
	 */
	return STATUS_SUCCESS;
}

