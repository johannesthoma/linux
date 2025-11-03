#include <asm-generic/div64.h>
#include <windows/api.h>

/* Those are defined in linux/types.h */
#define _UINTPTR_T_DEFINED 1
#define _INTPTR_T_DEFINED 1

#include <ntdef.h>
#include <ntddk.h>

struct win_timer_list {
    KTIMER ktimer;
    KDPC dpc;
    void (*function)(struct win_timer_list *data);
    ULONG_PTR expires;
};

unsigned long long win_jiffies(void)
{
	LARGE_INTEGER tick;

	KeQueryTickCount(&tick);
	tick.QuadPart *= KeQueryTimeIncrement();
	do_div(tick.QuadPart, 10000);	/* TODO: HZ */

	return tick.QuadPart;
}

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
	t->expires = 0;
}

void win_mod_timer(struct win_timer_list *timer, unsigned long long expires)
{
	LARGE_INTEGER nWaitTime = { .QuadPart = 0 };
	ULONG_PTR current_milisec = win_jiffies();

	timer->expires = expires;

	if (current_milisec >= expires)
		nWaitTime.QuadPart = -1;
	else {
		expires -= current_milisec;
		nWaitTime.QuadPart = -(((long long) expires) * 10 * 1000);
	}

	KeSetTimer(&timer->ktimer, nWaitTime, &timer->dpc);
}

void win_add_timer(struct win_timer_list *t)
{
	win_mod_timer(t, t->expires);
}

