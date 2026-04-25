#include <asm-generic/div64.h>
#include <windows/api.h>

#include "internal.h"

#if 0
/* Those are defined in linux/types.h */
#define _UINTPTR_T_DEFINED 1
#define _INTPTR_T_DEFINED 1

#include <ntdef.h>
#include <ntddk.h>
#endif

struct win_timer_list {
    KTIMER ktimer;
    KDPC dpc;
    void (*function)(struct win_timer_list *data);
};

static void __attribute__((stdcall)) win_timer_callback(PKDPC dpc, void *context, void *arg1_unused, void *arg2_unused)
{
	struct win_timer_list *timer = (struct win_timer_list *) context;

	if (timer->function != NULL) {
		timer->function(timer);
	}
}

struct win_timer_list *win_allocate_timer(void(*callback)(struct win_timer_list *timer))
{
	struct win_timer_list *timer;
	timer = win_allocate_memory(sizeof(*timer));
	if (timer == NULL)
		return NULL;

	timer->function = callback;
	KeInitializeTimer(&timer->ktimer);
	KeInitializeDpc(&timer->dpc, win_timer_callback, timer);

	return timer;
}

void win_del_timer(struct win_timer_list *t)
{
	KeCancelTimer(&t->ktimer);
}

void win_mod_timer_relative(struct win_timer_list *timer, unsigned long long expires)
{
	LARGE_INTEGER wait_time;

		/* negative values indicate relative time. The unit
		 * is 100ns so multiplying with 10000 gives 1ms.
		 */

	wait_time.QuadPart = -(expires * 10 * 1000);
	KeSetTimer(&timer->ktimer, wait_time, &timer->dpc);
}

