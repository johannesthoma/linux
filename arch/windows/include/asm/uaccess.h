static __always_inline unsigned long __must_check
raw_copy_to_user(void __user *to, const void *from, unsigned long n)
{
printk("raw_copy_to_user: from: %p to: %p n: %ld\n", from, to, n);

	memcpy(to, from, n);
        return 0;
}

static __always_inline unsigned long
raw_copy_from_user(void *to, const void __user *from, unsigned long n)
{
printk("raw_copy_from_user: from: %p to: %p n: %ld\n", from, to, n);

	memcpy(to, from, n);
        return 0;
}

#include <asm-generic/uaccess.h>

