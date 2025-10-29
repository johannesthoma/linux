# Compile selected Linux drivers as native Windows drivers (.SYS files)
#

# WINDOWS_ARCH ?= x86_64
WINDOWS_ARCH ?= i686
CROSS_COMPILE = $(WINDOWS_ARCH)-w64-mingw32-
# ?? shouldn't that be the subarch?
SRCARCH = $(ARCH)

# EXTRA_CFLAGS += -UWIN32
# needed by include/acpi/platform/acenv.h ...
# mingw does not define this:
KBUILD_CFLAGS += -g -D__linux__=1

# LDFLAGS_vmlinux := -L/home/johannes/.zeranoe/mingw-w64/i686/i686-w64-mingw32/lib -lntoskrnl -lhal -lntdll -lnetio
# DRIVER_ENTRY = _start_kernel
DRIVER_ENTRY = _DriverEntry
# KBUILD_LDFLAGS += -shared --subsystem native --dynamicbase --entry start_kernel
# This works (without script): need to write a lds script one day.
# i686-w64-mingw32-ld -shared --subsystem native --image-base 0xc0000000 --dynamicbase --nxcompat --file-alignment 0x200 --section-alignment 0x1000 --stack 0x100000 --gc-sections --exclude-all-symbols --entry _start_kernel --build-id=sha1 -o vmlinux --whole-archive vmlinux.a init/version-timestamp.o --no-whole-archive --start-group lib/lib.a arch/windows/lib/lib.a --end-group
# for linking with ntoskrnl, ... do the following (for now ...):
# libs must come at the end!
# /home/johannes/.zeranoe/mingw-w64/i686/bin/i686-w64-mingw32-ld -shared --subsystem native --image-base 0xc0000000 --dynamicbase --nxcompat --file-alignment 0x200 --section-alignment 0x1000 --stack 0x100000 --gc-sections --exclude-all-symbols --entry _DriverEntry --build-id=sha1 -o vmlinux --whole-archive vmlinux.a init/version-timestamp.o --no-whole-archive --start-group lib/lib.a arch/windows/lib/lib.a --end-group -L/home/johannes/.zeranoe/mingw-w64/i686/i686-w64-mingw32/lib -lntoskrnl -lhal -lntdll -lnetio

# Update: changed image-base:
# /home/johannes/.zeranoe/mingw-w64/i686/bin/i686-w64-mingw32-ld -g -shared --subsystem native --image-base 0x40000000 --dynamicbase --nxcompat --file-alignment 0x200 --section-alignment 0x1000 --stack 0x100000 --gc-sections --exclude-all-symbols --entry _DriverEntry --build-id=sha1 -o vmlinux --whole-archive vmlinux.a init/version-timestamp.o --no-whole-archive --start-group lib/lib.a arch/windows/lib/lib.a --end-group -L/home/johannes/.zeranoe/mingw-w64/i686/i686-w64-mingw32/lib -lntoskrnl -lhal -lntdll -lnetio

LDFLAGS_vmlinux += -shared --subsystem native --entry $(DRIVER_ENTRY) # --image-base 0x40000000 --dynamicbase --nxcompat --file-alignment 0x200 --section-alignment 0x1000 --stack 0x100000 --gc-sections --exclude-all-symbols --entry $(DRIVER_ENTRY)

# MinGW assumes printf format strings as supported by
# Windows (32 bit). We are using the Linux kernel
# vsnprintf.c implementation which supports far more
# format specifiers than MinGW would expect. Silence
# warnings about printk format specifiers.
KBUILD_CFLAGS += -Wno-format-extra-args -Wno-format
generated-y :=
generic-y :=

libs-y += arch/windows/lib/
core-y += arch/windows/kernel/
core-y += arch/windows/windows/

