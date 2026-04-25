/* later: */

PDEVICE_OBJECT drbd_bus_device;
PDEVICE_OBJECT drbd_physical_bus_device;

KEVENT bus_ready_event;

int windrbd_wait_for_bus_object(void)
{
	NTSTATUS status;

	status = KeWaitForSingleObject(&bus_ready_event, Executive, KernelMode, FALSE, NULL);

	if (status != STATUS_SUCCESS)
		return -1;

	return 0;
}

void windrbd_bus_is_ready(void)
{
	KeSetEvent(&bus_ready_event, 0, FALSE);
}

int windrbd_rescan_bus(void)
{
	if (drbd_physical_bus_device != NULL) {
		/* TODO: check if bus device was removed/added again */
		IoInvalidateDeviceRelations(drbd_physical_bus_device, BusRelations);
		return 0;
	}
	printk("Warning: physical bus device does not exist (yet)\n");
	return -1;
}

NTSTATUS
__attribute__((stdcall)) mvolAddDevice(IN PDRIVER_OBJECT DriverObject, IN PDEVICE_OBJECT PhysicalDeviceObject)
{
	UNICODE_STRING drbd_bus, drbd_bus_dos;
	NTSTATUS status;
	struct _DEVICE_OBJECT *bus_device;
	struct _BUS_EXTENSION *bus_extension;
	UNICODE_STRING bus_device_name;

	printk(KERN_INFO "AddDevice: PhysicalDeviceObject is %p\n", PhysicalDeviceObject);

	/* This assumes that the bus device object is the first
	 * object being attached, which is 'normally' the case.
	 */

	if (drbd_bus_device == NULL) {
		RtlInitUnicodeString(&drbd_bus, L"\\Device\\windrbd_bus_device");
		RtlInitUnicodeString(&drbd_bus_dos, L"\\DosDevices\\windrbd_bus_device");

		status = IoCreateDevice(DriverObject, sizeof(BUS_EXTENSION), &drbd_bus, FILE_DEVICE_BUS_EXTENDER, FILE_DEVICE_SECURE_OPEN, FALSE, &bus_device);
		if (status != STATUS_SUCCESS)
			printk("IoCreateDevice bus device returned %x\n", status);
		else
			printk("Bus device object created bus_device is %p\n", bus_device);

		if (bus_device == NULL) {
			printk("Could not create bus device - bus_device is NULL");
			return STATUS_INTERNAL_ERROR;
		}
		status = IoCreateSymbolicLink(&drbd_bus_dos, &drbd_bus);

		if (status != STATUS_SUCCESS)
			printk("IoCreateSymbolicLink bus device returned %x\n", status);
		else
			printk("Bus device object symlink created\n");

		bus_device->Flags |= DO_DIRECT_IO;                  // FIXME?
		bus_device->Flags |= DO_POWER_INRUSH;               // FIXME?

		bus_extension = (struct _BUS_EXTENSION*) bus_device->DeviceExtension;

		if (PhysicalDeviceObject != NULL) {
			status = IoRegisterDeviceInterface(PhysicalDeviceObject,
					&GUID_DEVCLASS_SCSIADAPTER,
					NULL,
					&bus_device_name);
			printk("IoRegisterDeviceInterface returned %x\n", status);
			printk("bus_device_name is %S\n", bus_device_name.Buffer);

			bus_extension->lower_device = IoAttachDeviceToDeviceStack(bus_device, PhysicalDeviceObject);
			if (bus_extension->lower_device == NULL)
				printk("IoAttachDeviceToDeviceStack failed.\n");
			else
				printk("IoAttachDeviceToDeviceStack returned object %p.\n", bus_extension->lower_device);
#if 0
			bus_extension->lower_device = NULL;
			printk("Set bus_extension->lower_device to NULL\n");
#endif
		} else {
			printk("PhysicalDeviceObject is NULL\n");
		}
		bus_device->Flags &= ~DO_DEVICE_INITIALIZING;

		drbd_bus_device = bus_device;
		drbd_physical_bus_device = PhysicalDeviceObject;
	}

	return STATUS_SUCCESS;
}
#endif
