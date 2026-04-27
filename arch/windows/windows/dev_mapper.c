#include "internal.h"
#include <linux/module.h>

static PDEVICE_OBJECT mapper;

static NTSTATUS create_mapper(PDEVICE_OBJECT device, PIRP irp, void *user_data)
{
	printk("create_mapper()\n");
	return STATUS_SUCCESS;
}

static NTSTATUS close_mapper(PDEVICE_OBJECT device, PIRP irp, void *user_data)
{
	printk("close_mapper()\n");
	return STATUS_SUCCESS;
}

static irp_handler_fn_t dispatch_mapper[IRP_MJ_MAXIMUM_FUNCTION] = {
	[IRP_MJ_CREATE] create_mapper,
	[IRP_MJ_CLOSE] close_mapper,
};

static int create_mapper_device(void)
{
	NTSTATUS status;

	printk("create_mapper_device()\n");
	status = create_device(L"device-mapper", FILE_DEVICE_UNKNOWN, &dispatch_mapper, NULL, &mapper);
	if (!NT_SUCCESS(status)) {
		printk("could not create device-mapper object, status is %08x\n", status);
		return -EINVAL;
	}
	return 0;

}

static void delete_mapper_device(void)
{
	printk("would delete mapper device here ...\n");
}

module_init(create_mapper_device);
module_exit(delete_mapper_device);
