#ifndef _WINDOWS_EXTABLE_H
#define _WINDOWS_EXTABLE_H

	/* from arch/x86 ... */
struct exception_table_entry {
        int insn, fixup, data;
};

#endif
