#include "internal.h"
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/atomic.h>
#include <uapi/linux/dm-ioctl.h>

static PDEVICE_OBJECT mapper;
static atomic_t num_open;

struct file *dev_mapper_control;

static NTSTATUS create_mapper(PDEVICE_OBJECT device, PIRP irp, void *user_data)
{
	struct file *f;
	printk("create_mapper()\n");

	if (dev_mapper_control) {
		atomic_inc(&num_open);
		return STATUS_SUCCESS;
	}
	f = filp_open("/dev/mapper/control", O_RDWR, 0);

	if (IS_ERR(f)) {
		if (PTR_ERR(f) == -ENOENT)
			printk("/dev/mapper/control not found. You probably want to configure CONFIG_INITRAMFS_SOURCE to arch/windows/windows_initramfs_cpio_list\n");

		return STATUS_NO_SUCH_FILE;
	}

	dev_mapper_control = f;
	atomic_inc(&num_open);
	return STATUS_SUCCESS;
}

static NTSTATUS close_mapper(PDEVICE_OBJECT device, PIRP irp, void *user_data)
{
	printk("close_mapper()\n");
	if (dev_mapper_control && !IS_ERR(dev_mapper_control) && atomic_dec_and_test(&num_open)) {
		filp_close(dev_mapper_control, NULL);
		dev_mapper_control = NULL;
	}
	return STATUS_SUCCESS;
}

static NTSTATUS ioctl_mapper(PDEVICE_OBJECT device, PIRP irp, void *user_data)
{
	struct _IO_STACK_LOCATION *s = IoGetCurrentIrpStackLocation(irp);

printk("1\n");
	if (!dev_mapper_control)
		return STATUS_NO_SUCH_FILE;

printk("1a\n");
	if (irp->AssociatedIrp.SystemBuffer == NULL)
		return STATUS_INVALID_DEVICE_REQUEST;

printk("2\n");
	if (s->Parameters.DeviceIoControl.InputBufferLength < sizeof(struct dm_ioctl) || s->Parameters.DeviceIoControl.OutputBufferLength < sizeof(struct dm_ioctl))
		return STATUS_BUFFER_TOO_SMALL;

printk("3\n");
	struct dm_ioctl *d = (struct dm_ioctl*) irp->AssociatedIrp.SystemBuffer;
	if (s->Parameters.DeviceIoControl.InputBufferLength < d->data_size || s->Parameters.DeviceIoControl.OutputBufferLength < d->data_size)
		return STATUS_BUFFER_TOO_SMALL;

printk("4\n");
	unsigned int linux_command = _IOWR(DM_IOCTL, (s->Parameters.DeviceIoControl.IoControlCode >> 2) & 0xff, struct dm_ioctl);
printk("linux_command is %x\n", linux_command);
	if (vfs_ioctl(dev_mapper_control, linux_command, (unsigned long) d) < 0)
		return STATUS_INVALID_DEVICE_REQUEST;

printk("5\n");
	irp->IoStatus.Information = d->data_size;
	return STATUS_SUCCESS;
}

static irp_handler_fn_t dispatch_mapper[IRP_MJ_MAXIMUM_FUNCTION] = {
	[IRP_MJ_CREATE] create_mapper,
	[IRP_MJ_CLOSE] close_mapper,
	[IRP_MJ_DEVICE_CONTROL] ioctl_mapper,
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
