#include <linux/types.h>
#include <asm/page.h>
#include <linux/align.h>

/* Those are defined in linux/types.h */
#define _UINTPTR_T_DEFINED 1
#define _INTPTR_T_DEFINED 1

/* The Exxxx macros are defined in various Linux headers (with
 * different values) do not use the ReactOS header's errno.h.
 */
#define _INC_ERRNO 1
#define NOMINMAX 1

#define DBG 0
#define DEVL 0
/* No: we aren't a Microsoft Windows compiler: */
// #define _MSC_VER 1300

/* Windows Server 2003: What ReactOS is currently on: 
 * We don't want to use any newer features from the Windows kernel
 * for now ..
 */
#define _WIN32_WINNT 0x502

#include <wdm.h>

extern PDRIVER_OBJECT driver_object;
