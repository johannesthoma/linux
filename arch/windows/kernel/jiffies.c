#include <linux/jiffies.h>

unsigned long volatile __cacheline_aligned_in_smp __jiffy_arch_data jiffies;

