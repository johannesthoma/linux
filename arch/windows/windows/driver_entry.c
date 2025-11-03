#include <ntdef.h>
#include <ntddk.h>

// typedef int NTSTATUS;
// #define STATUS_SUCCESS 0

// extern int DbgPrint(const char *fmt, ...);

// NTSTATUS __attribute__((stdcall)) DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING registry_path)

extern void start_kernel(void);

NTSTATUS __attribute__((stdcall)) DriverEntry(void *DriverObject, void *registry_path)
{
	DbgPrint("Hallo Linux!!\n");
	start_kernel();

	return STATUS_SUCCESS;
}

