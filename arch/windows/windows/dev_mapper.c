#include "internal.h"
#include <linux/module.h>
#include <linux/fs.h>

static PDEVICE_OBJECT mapper;

struct file *dev_mapper_control;

static NTSTATUS create_mapper(PDEVICE_OBJECT device, PIRP irp, void *user_data)
{
	printk("create_mapper()\n");

	// dev_mapper_control = filp_open("/dev/console", O_RDWR, 0);
	// dev_mapper_control = filp_open("/karin/zak", O_RDWR, 0);
	// dev_mapper_control = filp_open("/dev/lebt", O_RDWR, 0);
	dev_mapper_control = filp_open("/dev/mapper/control", O_RDWR, 0);
		/* TODO: have a linux_to_windows error func */
printk("dev_mapper_control is %p\n", dev_mapper_control);
	if (IS_ERR(dev_mapper_control)) {
printk("is err...\n");
		return STATUS_OBJECT_NAME_NOT_FOUND; /* or so ... */
	}
printk("is ok...\n");
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
	status = create_device(L"device_mapper", FILE_DEVICE_UNKNOWN, &dispatch_mapper, NULL, &mapper);
	if (!NT_SUCCESS(status)) {
		printk("could not create device-mapper object, status is %08x\n", status);
		return -EINVAL;
	}
	/* Do init here ... */
	mapper->Flags &= ~DO_DEVICE_INITIALIZING;

	return 0;

}

static void delete_mapper_device(void)
{
	printk("would delete mapper device here ...\n");
}

module_init(create_mapper_device);
module_exit(delete_mapper_device);
