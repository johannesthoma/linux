#ifndef WINDOWS_PROCESSOR_H
#define WINDOWS_PROCESSOR_H

#include <asm/pgtable_types.h>
#include <asm/segment.h>
#include <asm/desc_defs.h>
#include <asm/special_insns.h>
#include <asm/fpu/types.h>
#include <asm/msr.h>
#include <asm/percpu.h>

#define HBP_NUM 4

/* TODO: */
#define cpu_relax() do { } while (0)

struct thread_struct {
	/* Cached TLS descriptors: */
	struct desc_struct	tls_array[GDT_ENTRY_TLS_ENTRIES];
#ifdef CONFIG_X86_32
	unsigned long		sp0;
#endif
	unsigned long		sp;
#ifdef CONFIG_X86_32
	unsigned long		sysenter_cs;
#else
	unsigned short		es;
	unsigned short		ds;
	unsigned short		fsindex;
	unsigned short		gsindex;
#endif

#ifdef CONFIG_X86_64
	unsigned long		fsbase;
	unsigned long		gsbase;
#else
	/*
	 * XXX: this could presumably be unsigned short.  Alternatively,
	 * 32-bit kernels could be taught to use fsindex instead.
	 */
	unsigned long fs;
	unsigned long gs;
#endif

	/* Save middle states of ptrace breakpoints */
	struct perf_event	*ptrace_bps[HBP_NUM];
	/* Debug status used for traps, single steps, etc... */
	unsigned long           virtual_dr6;
	/* Keep track of the exact dr7 value set by the user */
	unsigned long           ptrace_dr7;
	/* Fault info: */
	unsigned long		cr2;
	unsigned long		trap_nr;
	unsigned long		error_code;
#ifdef CONFIG_VM86
	/* Virtual 86 mode info */
	struct vm86		*vm86;
#endif
	/* IO permissions: */
	struct io_bitmap	*io_bitmap;

	/*
	 * IOPL. Privilege level dependent I/O permission which is
	 * emulated via the I/O bitmap to prevent user space from disabling
	 * interrupts.
	 */
	unsigned long		iopl_emul;

	unsigned int		iopl_warn:1;
	unsigned int		sig_on_uaccess_err:1;

	/*
	 * Protection Keys Register for Userspace.  Loaded immediately on
	 * context switch. Store it in thread_struct to avoid a lookup in
	 * the tasks's FPU xstate buffer. This value is only valid when a
	 * task is scheduled out. For 'current' the authoritative source of
	 * PKRU is the hardware itself.
	 */
	u32			pkru;

#ifdef CONFIG_X86_USER_SHADOW_STACK
	unsigned long		features;
	unsigned long		features_locked;

	struct thread_shstk	shstk;
#endif

	/* Floating point and extended processor state */
	struct fpu		fpu;
	/*
	 * WARNING: 'fpu' is dynamically-sized.  It *MUST* be at
	 * the end.
	 */
};

#define KSTK_ESP(task)         (0)
#define KSTK_EIP(task)         (0)

#define INIT_THREAD  {  }

/*
 * This decides where the kernel will search for a free chunk of vm
 * space during mmap's.
 */
#define __TASK_UNMAPPED_BASE(task_size) (PAGE_ALIGN(task_size / 3))
#define TASK_UNMAPPED_BASE              __TASK_UNMAPPED_BASE(TASK_SIZE_LOW)

#define task_pt_regs(task) \
({                                                                      \
        unsigned long __ptr = (unsigned long)task_stack_page(task);     \
        __ptr += THREAD_SIZE - TOP_OF_KERNEL_STACK_PADDING;             \
        ((struct pt_regs *)__ptr) - 1;                                  \
})

static inline unsigned long __get_wchan(struct task_struct *p)
{
	return 0;
}

struct entry_stack {
        char    stack[PAGE_SIZE];
};

struct entry_stack_page {
        struct entry_stack stack;
} __aligned(PAGE_SIZE);

/*
 * Note that while the legacy 'TSS' name comes from 'Task State Segment',
 * on modern x86 CPUs the TSS also holds information important to 64-bit mode,
 * unrelated to the task-switch mechanism:
 */
#ifdef CONFIG_X86_32
/* This is the TSS defined by the hardware. */
struct x86_hw_tss {
	unsigned short		back_link, __blh;
	unsigned long		sp0;
	unsigned short		ss0, __ss0h;
	unsigned long		sp1;

	/*
	 * We don't use ring 1, so ss1 is a convenient scratch space in
	 * the same cacheline as sp0.  We use ss1 to cache the value in
	 * MSR_IA32_SYSENTER_CS.  When we context switch
	 * MSR_IA32_SYSENTER_CS, we first check if the new value being
	 * written matches ss1, and, if it's not, then we wrmsr the new
	 * value and update ss1.
	 *
	 * The only reason we context switch MSR_IA32_SYSENTER_CS is
	 * that we set it to zero in vm86 tasks to avoid corrupting the
	 * stack if we were to go through the sysenter path from vm86
	 * mode.
	 */
	unsigned short		ss1;	/* MSR_IA32_SYSENTER_CS */

	unsigned short		__ss1h;
	unsigned long		sp2;
	unsigned short		ss2, __ss2h;
	unsigned long		__cr3;
	unsigned long		ip;
	unsigned long		flags;
	unsigned long		ax;
	unsigned long		cx;
	unsigned long		dx;
	unsigned long		bx;
	unsigned long		sp;
	unsigned long		bp;
	unsigned long		si;
	unsigned long		di;
	unsigned short		es, __esh;
	unsigned short		cs, __csh;
	unsigned short		ss, __ssh;
	unsigned short		ds, __dsh;
	unsigned short		fs, __fsh;
	unsigned short		gs, __gsh;
	unsigned short		ldt, __ldth;
	unsigned short		trace;
	unsigned short		io_bitmap_base;

} __attribute__((packed));
#else
struct x86_hw_tss {
	u32			reserved1;
	u64			sp0;
	u64			sp1;

	/*
	 * Since Linux does not use ring 2, the 'sp2' slot is unused by
	 * hardware.  entry_SYSCALL_64 uses it as scratch space to stash
	 * the user RSP value.
	 */
	u64			sp2;

	u64			reserved2;
	u64			ist[7];
	u32			reserved3;
	u32			reserved4;
	u16			reserved5;
	u16			io_bitmap_base;

} __attribute__((packed));
#endif

#define IO_BITMAP_BITS			65536
#define IO_BITMAP_BYTES			(IO_BITMAP_BITS / BITS_PER_BYTE)
#define IO_BITMAP_LONGS			(IO_BITMAP_BYTES / sizeof(long))

# define __KERNEL_TSS_LIMIT     \
        (offsetof(struct tss_struct, x86_tss) + sizeof(struct x86_hw_tss) - 1)

/*
 * All IO bitmap related data stored in the TSS:
 */
struct x86_io_bitmap {
	/* The sequence number of the last active bitmap. */
	u64			prev_sequence;

	/*
	 * Store the dirty size of the last io bitmap offender. The next
	 * one will have to do the cleanup as the switch out to a non io
	 * bitmap user will just set x86_tss.io_bitmap_base to a value
	 * outside of the TSS limit. So for sane tasks there is no need to
	 * actually touch the io_bitmap at all.
	 */
	unsigned int		prev_max;

	/*
	 * The extra 1 is there because the CPU will access an
	 * additional byte beyond the end of the IO permission
	 * bitmap. The extra byte must be all 1 bits, and must
	 * be within the limit.
	 */
	unsigned long		bitmap[IO_BITMAP_LONGS + 1];

	/*
	 * Special I/O bitmap to emulate IOPL(3). All bytes zero,
	 * except the additional byte at the end.
	 */
	unsigned long		mapall[IO_BITMAP_LONGS + 1];
};

struct tss_struct {
        /*
         * The fixed hardware portion.  This must not cross a page boundary
         * at risk of violating the SDM's advice and potentially triggering
         * errata.
         */
        struct x86_hw_tss       x86_tss;

        struct x86_io_bitmap    io_bitmap;
} __aligned(PAGE_SIZE);

DECLARE_PER_CPU_PAGE_ALIGNED(struct tss_struct, cpu_tss_rw);

extern void start_thread(struct pt_regs *regs, unsigned long new_ip,
					       unsigned long new_sp);

/*
 * CPU type and hardware bug flags. Kept separately for each CPU.
 */

struct cpuinfo_topology {
	// Real APIC ID read from the local APIC
	u32			apicid;
	// The initial APIC ID provided by CPUID
	u32			initial_apicid;

	// Physical package ID
	u32			pkg_id;

	// Physical die ID on AMD, Relative on Intel
	u32			die_id;

	// Compute unit ID - AMD specific
	u32			cu_id;

	// Core ID relative to the package
	u32			core_id;

	// Logical ID mappings
	u32			logical_pkg_id;
	u32			logical_die_id;

	// Cache level topology IDs
	u32			llc_id;
	u32			l2c_id;
};

struct cpuinfo_x86 {
	__u8			x86;		/* CPU family */
	__u8			x86_vendor;	/* CPU vendor */
	__u8			x86_model;
	__u8			x86_stepping;
#ifdef CONFIG_X86_64
	/* Number of 4K pages in DTLB/ITLB combined(in pages): */
	int			x86_tlbsize;
#endif
#ifdef CONFIG_X86_VMX_FEATURE_NAMES
	__u32			vmx_capability[NVMXINTS];
#endif
	__u8			x86_virt_bits;
	__u8			x86_phys_bits;
	/* CPUID returned core id bits: */
	__u8			x86_coreid_bits;
	/* Max extended CPUID function supported: */
	__u32			extended_cpuid_level;
	/* Maximum supported CPUID level, -1=no CPUID: */
	int			cpuid_level;
	/*
	 * Align to size of unsigned long because the x86_capability array
	 * is passed to bitops which require the alignment. Use unnamed
	 * union to enforce the array is aligned to size of unsigned long.
	 */
	union {
		__u32		x86_capability[NCAPINTS + NBUGINTS];
		unsigned long	x86_capability_alignment;
	};
	char			x86_vendor_id[16];
	char			x86_model_id[64];
	struct cpuinfo_topology	topo;
	/* in KB - valid for CPUS which support this call: */
	unsigned int		x86_cache_size;
	int			x86_cache_alignment;	/* In bytes */
	/* Cache QoS architectural values, valid only on the BSP: */
	int			x86_cache_max_rmid;	/* max index */
	int			x86_cache_occ_scale;	/* scale to bytes */
	int			x86_cache_mbm_width_offset;
	int			x86_power;
	unsigned long		loops_per_jiffy;
	/* protected processor identification number */
	u64			ppin;
	/* cpuid returned max cores value: */
	u16			x86_max_cores;
	u16			x86_clflush_size;
	/* number of cores as seen by the OS: */
	u16			booted_cores;
	/* Index into per_cpu list: */
	u16			cpu_index;
	/*  Is SMT active on this core? */
	bool			smt_active;
	u32			microcode;
	/* Address space bits used by the cache internally */
	u8			x86_cache_bits;
	unsigned		initialized : 1;
} __randomize_layout;

#define X86_VENDOR_INTEL	0
#define X86_VENDOR_CYRIX	1
#define X86_VENDOR_AMD		2
#define X86_VENDOR_UMC		3
#define X86_VENDOR_CENTAUR	5
#define X86_VENDOR_TRANSMETA	7
#define X86_VENDOR_NSC		8
#define X86_VENDOR_HYGON	9
#define X86_VENDOR_ZHAOXIN	10
#define X86_VENDOR_VORTEX	11
#define X86_VENDOR_NUM		12

#define X86_VENDOR_UNKNOWN	0xff

extern struct cpuinfo_x86	boot_cpu_data;
extern struct cpuinfo_x86	new_cpu_data;

extern __u32			cpu_caps_cleared[NCAPINTS + NBUGINTS];
extern __u32			cpu_caps_set[NCAPINTS + NBUGINTS];

#ifdef CONFIG_SMP
DECLARE_PER_CPU_READ_MOSTLY(struct cpuinfo_x86, cpu_info);
#define cpu_data(cpu)		per_cpu(cpu_info, cpu)
#else
#define cpu_info		boot_cpu_data
#define cpu_data(cpu)		boot_cpu_data
#endif

/*
 * Friendlier CR3 helpers.
 */
static inline unsigned long read_cr3_pa(void)
{
	return __read_cr3() & CR3_ADDR_MASK;
}

static inline unsigned long native_read_cr3_pa(void)
{
	return __native_read_cr3() & CR3_ADDR_MASK;
}

#endif
