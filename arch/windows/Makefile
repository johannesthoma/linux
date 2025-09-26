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
KBUILD_CFLAGS += -D__linux__=1

# MinGW assumes printf format strings as supported by
# Windows (32 bit). We are using the Linux kernel
# vsnprintf.c implementation which supports far more
# format specifiers than MinGW would expect. Silence
# warnings about printk format specifiers.
KBUILD_CFLAGS += -Wno-format-extra-args -Wno-format
generated-y :=
generic-y :=
