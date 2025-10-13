#include <ntdef.h>
#include <ntddk.h>
#include <ntdddisk.h>

// typedef int NTSTATUS;
// #define STATUS_SUCCESS 0

// extern int DbgPrint(const char *fmt, ...);

// NTSTATUS __attribute__((stdcall)) DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING registry_path)
NTSTATUS __attribute__((stdcall)) DriverEntry(void *DriverObject, void *registry_path)
{
	DbgPrint("Hallo Linux!!\n");
	return STATUS_SUCCESS;
}

