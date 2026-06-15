Linux kernel: Windows support
=============================

This is the development branch for building Linux as a Windows/ReactOS
driver. If you want to download a regular Linux kernel instead, go
to https://www.kernel.org/ or https://github.com/torvalds/linux

Build requirements
------------------

You need a working i686-w64-mingw32-gcc as well as make installed.
I usually build on Linux, but cygwin/WSL2 is probably also ok.

Building
--------

To build Linux as a Windows driver, do:

    make ARCH=windows i686_windows_reactos_defconfig
    make -j $(nproc) ARCH=windows

It produces a vmlinux, which can be loaded as a Windows driver.
Copy the file to a Windows machine and rename it to linux.sys.

Installing
----------

To configure a system service for the linux kernel as a driver, do
(on Windows or ReactOS as Administrator, you need to alter the
path):

    sc create linux type= kernel binpath= c:\path\to\linux.sys

To start the kernel, do

    sc start linux

Current state
-------------

What currently works is loading and booting Linux on 32-bit
Windows systems. If you attach a debugger (or dump the serial
console for ReactOS) you can see the kernel log messages showing
the boot process of the Linux kernel. In particular, running
Linux binaries is being worked on, but currently not supported.

If you want to reach me, send me an eMail to

    johannes@johannesthoma.com

Also, I maintain a project website at:

    https://jtsoft.at/wsl4/

Best wishes and happy hacking,

 - Johannes Khoshnazar-Thoma


