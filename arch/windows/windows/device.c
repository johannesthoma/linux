/*
        Copyright(C) 2017-2018, Johannes Thoma <johannes@johannesthoma.com>
        Copyright(C) 2017-2018, LINBIT HA-Solutions GmbH  <office@linbit.com>
	Copyright(C) 2007-2016, ManTechnology Co., LTD.
	Copyright(C) 2007-2016, wdrbd@mantech.co.kr

	Windows DRBD is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2, or (at your option)
	any later version.

	Windows DRBD is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Windows DRBD; see the file COPYING. If not, write to
	the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <linux/types.h>
#include <linux/workqueue.h>
#include <linux/printk.h>
#include <linux/slab.h>

#include "internal.h"

#include <ntifs.h>
#include <rtltypes.h>
#include <strsafe.h>

#define TO_UNICODE(s) WIDEN2(s)
#define WIDEN2(s) L##s

typedef NTSTATUS (*irp_handler_fn_t)(struct _DEVICE_OBJECT *device, struct _IRP *irp, void *user_data);

struct device_extension {
	irp_handler_fn_t (*dispatch_table)[IRP_MJ_MAXIMUM_FUNCTION];
	void *user_data;
};

NTSTATUS set_admin_only_permission(struct _DEVICE_OBJECT *obj)
{
	HANDLE h;
	NTSTATUS status;
	SECURITY_DESCRIPTOR desc;
	int Count;
	PACL Dacl;

	printk("About to set admin only permissions to root object ...\n");
	status = ObOpenObjectByPointer(obj, 0, NULL, WRITE_DAC, 0, KernelMode, &h);
	if (!NT_SUCCESS(status)) {
		printk("Can't open root object, status is %08X\n", status);
		return status;
	}

		/* This was taken from ntoskrnl/mm/pagefile.c of the ReactOS
		 * kernel.
		 */

	status = RtlCreateSecurityDescriptor(&desc, SECURITY_DESCRIPTOR_REVISION);
	if (!NT_SUCCESS(status)) {
		printk("Can't create security descriptor, status is %08X\n", status);
		ZwClose(h);
		return status;
	}

	/* Create the DACL: we will only allow two SIDs */
	Count = sizeof(ACL) + (sizeof(ACE) + RtlLengthSid(SeExports->SeLocalSystemSid)) +
			      (sizeof(ACE) + RtlLengthSid(SeExports->SeAliasAdminsSid));
	Dacl = kmalloc(Count, GFP_KERNEL);
	if (Dacl == NULL) {
		printk("Could not allocate DACL\n");
		ZwClose(h);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	/* Initialize the DACL */
	status = RtlCreateAcl(Dacl, Count, ACL_REVISION);
	if (!NT_SUCCESS(status)) {
		printk("Can't create security DACL, status is %08X\n", status);
		kfree(Dacl);
		ZwClose(h);
		return status;
	}

	/* Grant full access to admins */
	status = RtlAddAccessAllowedAce(Dacl, ACL_REVISION, FILE_ALL_ACCESS, SeExports->SeAliasAdminsSid);
	if (!NT_SUCCESS(status)) {
		printk("Can't add admin to DACL, status is %08X\n", status);
		kfree(Dacl);
		ZwClose(h);
		return status;
	}

	/* Grant full access to SYSTEM */
	status = RtlAddAccessAllowedAce(Dacl, ACL_REVISION, FILE_ALL_ACCESS, SeExports->SeLocalSystemSid);
	if (!NT_SUCCESS(status)) {
		printk("Can't add system to DACL, status is %08X\n", status);
		kfree(Dacl);
		ZwClose(h);
		return status;
	}

	/* Attach the DACL to the security descriptor */
	status = RtlSetDaclSecurityDescriptor(&desc, TRUE, Dacl, FALSE);
	if (!NT_SUCCESS(status)) {
		printk("Can't add DACL to security descriptor, status is %08X\n", status);
		kfree(Dacl);
		ZwClose(h);
		return status;
	}
	status = ZwSetSecurityObject(h, DACL_SECURITY_INFORMATION, &desc);

	if (!NT_SUCCESS(status)) {
		printk("Can't set security object, status is %08X\n", status);
		kfree(Dacl);
		ZwClose(h);
		return status;
	}

	kfree(Dacl);
	ZwClose(h);
	printk("Succeeded\n");

	return status;
}

NTSTATUS create_device(const wchar_t *name, irp_handler_fn_t (*dispatch_table)[IRP_MJ_MAXIMUM_FUNCTION], void *user_data, struct _DEVICE_OBJECT **d)
{
	NTSTATUS status;
	PDEVICE_OBJECT deviceObject;
	UNICODE_STRING nameUnicode, linkUnicode;
	wchar_t tmp[100], tmp2[100];
	struct device_extension *ext;

	_snwprintf(tmp, ARRAY_SIZE(tmp), L"\\Device\\%s", name);
	tmp[99] = 0;
	RtlInitUnicodeString(&nameUnicode, tmp);

	printk("About to create device %S with default permissions\n", nameUnicode.Buffer);
	status = IoCreateDevice(driver_object, sizeof(struct device_extension),
		        &nameUnicode, FILE_DEVICE_UNKNOWN,
			FILE_DEVICE_SECURE_OPEN, FALSE, &deviceObject);

	if (!NT_SUCCESS(status))
	{
		printk("Can't create root, err=%x\n", status);
		return status;
	}

	_snwprintf(tmp2, ARRAY_SIZE(tmp2), L"\\DosDevices\\%s", name);
	RtlInitUnicodeString(&linkUnicode, tmp2);
	printk("About to create symbolic link from %S to %S\n", linkUnicode.Buffer, nameUnicode.Buffer);
	status = IoCreateSymbolicLink(&linkUnicode, &nameUnicode);
	if (!NT_SUCCESS(status))
	{
		printk("cannot create symbolic link, err=%x\n", status);
		IoDeleteDevice(deviceObject);
		return status;
	}
	ext = deviceObject->DeviceExtension;
	ext->dispatch_table = dispatch_table;
	ext->user_data = user_data;

	if (d)
		*d = deviceObject;

	return STATUS_SUCCESS;
}

