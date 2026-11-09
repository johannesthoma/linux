# 0 "arch/windows/entry/entry_32.S"
# 1 "/home/johannes/Linbit/linux//"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "././include/linux/compiler-version.h" 1
# 0 "<command-line>" 2
# 1 "././include/linux/kconfig.h" 1




# 1 "./include/generated/autoconf.h" 1
# 6 "././include/linux/kconfig.h" 2
# 0 "<command-line>" 2
# 1 "arch/windows/entry/entry_32.S"
# 32 "arch/windows/entry/entry_32.S"
# 1 "./include/linux/linkage.h" 1




# 1 "./include/linux/compiler_types.h" 1
# 6 "./include/linux/linkage.h" 2
# 1 "./include/linux/stringify.h" 1
# 7 "./include/linux/linkage.h" 2
# 1 "./include/linux/export.h" 1




# 1 "./include/linux/compiler.h" 1
# 251 "./include/linux/compiler.h"
# 1 "./arch/windows/include/asm/rwonce.h" 1
# 1 "./include/asm-generic/rwonce.h" 1
# 2 "./arch/windows/include/asm/rwonce.h" 2
# 252 "./include/linux/compiler.h" 2
# 6 "./include/linux/export.h" 2
# 1 "./include/linux/linkage.h" 1
# 7 "./include/linux/export.h" 2
# 8 "./include/linux/linkage.h" 2
# 1 "./arch/windows/include/asm/linkage.h" 1





# 1 "./arch/windows/include/asm/ibt.h" 1




# 1 "./include/linux/types.h" 1





# 1 "./include/uapi/linux/types.h" 1




# 1 "./arch/windows/include/generated/uapi/asm/types.h" 1
# 1 "./include/uapi/asm-generic/types.h" 1






# 1 "./include/asm-generic/int-ll64.h" 1
# 11 "./include/asm-generic/int-ll64.h"
# 1 "./include/uapi/asm-generic/int-ll64.h" 1
# 12 "./include/uapi/asm-generic/int-ll64.h"
# 1 "./arch/windows/include/generated/uapi/asm/bitsperlong.h" 1
# 1 "./include/asm-generic/bitsperlong.h" 1




# 1 "./include/uapi/asm-generic/bitsperlong.h" 1
# 6 "./include/asm-generic/bitsperlong.h" 2
# 2 "./arch/windows/include/generated/uapi/asm/bitsperlong.h" 2
# 13 "./include/uapi/asm-generic/int-ll64.h" 2
# 12 "./include/asm-generic/int-ll64.h" 2
# 8 "./include/uapi/asm-generic/types.h" 2
# 2 "./arch/windows/include/generated/uapi/asm/types.h" 2
# 6 "./include/uapi/linux/types.h" 2
# 7 "./include/linux/types.h" 2
# 6 "./arch/windows/include/asm/ibt.h" 2
# 7 "./arch/windows/include/asm/linkage.h" 2
# 9 "./include/linux/linkage.h" 2
# 33 "arch/windows/entry/entry_32.S" 2
# 1 "./include/linux/err.h" 1







# 1 "./arch/windows/include/generated/uapi/asm/errno.h" 1
# 1 "./include/uapi/asm-generic/errno.h" 1




# 1 "./include/uapi/asm-generic/errno-base.h" 1
# 6 "./include/uapi/asm-generic/errno.h" 2
# 2 "./arch/windows/include/generated/uapi/asm/errno.h" 2
# 9 "./include/linux/err.h" 2
# 34 "arch/windows/entry/entry_32.S" 2
# 1 "./arch/windows/include/asm/thread_info.h" 1
# 35 "arch/windows/entry/entry_32.S" 2
# 1 "./arch/windows/include/asm/irqflags.h" 1




# 1 "./arch/windows/include/asm/processor-flags.h" 1




# 1 "./arch/windows/include/uapi/asm/processor-flags.h" 1





# 1 "./include/linux/const.h" 1



# 1 "./include/vdso/const.h" 1




# 1 "./include/uapi/linux/const.h" 1
# 6 "./include/vdso/const.h" 2
# 5 "./include/linux/const.h" 2
# 7 "./arch/windows/include/uapi/asm/processor-flags.h" 2
# 6 "./arch/windows/include/asm/processor-flags.h" 2
# 1 "./include/linux/mem_encrypt.h" 1
# 7 "./arch/windows/include/asm/processor-flags.h" 2
# 6 "./arch/windows/include/asm/irqflags.h" 2
# 36 "arch/windows/entry/entry_32.S" 2
# 1 "./arch/windows/include/generated/uapi/asm/errno.h" 1
# 37 "arch/windows/entry/entry_32.S" 2

# 1 "./arch/windows/include/asm/segment.h" 1





# 1 "./arch/windows/include/asm/alternative.h" 1






# 1 "./arch/windows/include/asm/asm.h" 1
# 130 "./arch/windows/include/asm/asm.h"
# 1 "./arch/windows/include/asm/extable_fixup_types.h" 1
# 131 "./arch/windows/include/asm/asm.h" 2
# 8 "./arch/windows/include/asm/alternative.h" 2
# 344 "./arch/windows/include/asm/alternative.h"
 .macro LOCK_PREFIX
 .endm






.macro ANNOTATE_IGNORE_ALTERNATIVE
 .Lannotate_\@:
 .pushsection .discard.ignore_alts
 .long .Lannotate_\@
 .popsection
.endm







.macro altinstr_entry orig alt ft_flags orig_len alt_len
 .long \orig - .
 .long \alt - .
 .4byte \ft_flags
 .byte \orig_len
 .byte \alt_len
.endm







.macro ALTERNATIVE oldinstr, newinstr, ft_flags
140:
 \oldinstr
141:
 .skip -(((144f-143f)-(141b-140b)) > 0) * ((144f-143f)-(141b-140b)),0x90
142:

 .pushsection .altinstructions,"a"
 altinstr_entry 140b,143f,\ft_flags,142b-140b,144f-143f
 .popsection

 .pushsection .altinstr_replacement,"ax"
143:
 \newinstr
144:
 .popsection
.endm
# 417 "./arch/windows/include/asm/alternative.h"
.macro ALTERNATIVE_2 oldinstr, newinstr1, ft_flags1, newinstr2, ft_flags2
140:
 \oldinstr
141:
 .skip -((((144f-143f) ^ (((144f-143f) ^ (145f-144f)) & -(-((144f-143f) < (145f-144f))))) - (141b-140b)) > 0) * (((144f-143f) ^ (((144f-143f) ^ (145f-144f)) & -(-((144f-143f) < (145f-144f))))) - (141b-140b)),0x90

142:

 .pushsection .altinstructions,"a"
 altinstr_entry 140b,143f,\ft_flags1,142b-140b,144f-143f
 altinstr_entry 140b,144f,\ft_flags2,142b-140b,145f-144f
 .popsection

 .pushsection .altinstr_replacement,"ax"
143:
 \newinstr1
144:
 \newinstr2
145:
 .popsection
.endm

.macro ALTERNATIVE_3 oldinstr, newinstr1, ft_flags1, newinstr2, ft_flags2, newinstr3, ft_flags3
140:
 \oldinstr
141:
 .skip -(((((((144f-143f) ^ (((144f-143f) ^ (145f-144f)) & -(-((144f-143f) < (145f-144f)))))) ^ (((((144f-143f) ^ (((144f-143f) ^ (145f-144f)) & -(-((144f-143f) < (145f-144f)))))) ^ (146f-145f)) & -(-((((144f-143f) ^ (((144f-143f) ^ (145f-144f)) & -(-((144f-143f) < (145f-144f)))))) < (146f-145f)))))) - (141b-140b)) > 0) * ((((((144f-143f) ^ (((144f-143f) ^ (145f-144f)) & -(-((144f-143f) < (145f-144f)))))) ^ (((((144f-143f) ^ (((144f-143f) ^ (145f-144f)) & -(-((144f-143f) < (145f-144f)))))) ^ (146f-145f)) & -(-((((144f-143f) ^ (((144f-143f) ^ (145f-144f)) & -(-((144f-143f) < (145f-144f)))))) < (146f-145f)))))) - (141b-140b)),0x90

142:

 .pushsection .altinstructions,"a"
 altinstr_entry 140b,143f,\ft_flags1,142b-140b,144f-143f
 altinstr_entry 140b,144f,\ft_flags2,142b-140b,145f-144f
 altinstr_entry 140b,145f,\ft_flags3,142b-140b,146f-145f
 .popsection

 .pushsection .altinstr_replacement,"ax"
143:
 \newinstr1
144:
 \newinstr2
145:
 \newinstr3
146:
 .popsection
.endm
# 7 "./arch/windows/include/asm/segment.h" 2
# 167 "./arch/windows/include/asm/segment.h"
# 1 "./arch/windows/include/asm/cache.h" 1
# 1 "./include/asm-generic/cache.h" 1
# 2 "./arch/windows/include/asm/cache.h" 2
# 168 "./arch/windows/include/asm/segment.h" 2
# 39 "arch/windows/entry/entry_32.S" 2
# 1 "./arch/windows/include/asm/smp.h" 1
# 40 "arch/windows/entry/entry_32.S" 2
# 1 "./arch/windows/include/asm/percpu.h" 1
# 41 "arch/windows/entry/entry_32.S" 2

# 1 "./arch/windows/include/asm/irq_vectors.h" 1




# 1 "./include/linux/threads.h" 1
# 6 "./arch/windows/include/asm/irq_vectors.h" 2
# 43 "arch/windows/entry/entry_32.S" 2
# 1 "./arch/windows/include/asm/cpufeatures.h" 1





# 1 "./arch/windows/include/asm/required-features.h" 1
# 7 "./arch/windows/include/asm/cpufeatures.h" 2



# 1 "./arch/windows/include/asm/disabled-features.h" 1
# 11 "./arch/windows/include/asm/cpufeatures.h" 2
# 44 "arch/windows/entry/entry_32.S" 2


# 1 "./arch/windows/include/asm/smap.h" 1
# 12 "./arch/windows/include/asm/smap.h"
# 1 "./arch/windows/include/asm/nops.h" 1
# 13 "./arch/windows/include/asm/smap.h" 2
# 47 "arch/windows/entry/entry_32.S" 2
# 1 "./arch/windows/include/asm/frame.h" 1
# 93 "./arch/windows/include/asm/frame.h"
.macro ENCODE_FRAME_POINTER ptregs_offset=0
.endm
# 48 "arch/windows/entry/entry_32.S" 2
# 1 "./arch/windows/include/asm/trapnr.h" 1
# 49 "arch/windows/entry/entry_32.S" 2
# 1 "./arch/windows/include/asm/nospec-branch.h" 1





# 1 "./include/linux/static_key.h" 1
# 1 "./include/linux/jump_label.h" 1
# 2 "./include/linux/static_key.h" 2
# 7 "./arch/windows/include/asm/nospec-branch.h" 2
# 1 "./include/linux/objtool.h" 1




# 1 "./include/linux/objtool_types.h" 1
# 6 "./include/linux/objtool.h" 2
# 162 "./include/linux/objtool.h"
.macro UNWIND_HINT type:req sp_reg=0 sp_offset=0 signal=0
.endm
.macro STACK_FRAME_NON_STANDARD func:req
.endm
.macro ANNOTATE_NOENDBR
.endm
.macro REACHABLE
.endm
# 8 "./arch/windows/include/asm/nospec-branch.h" 2




# 1 "./arch/windows/include/asm/msr-index.h" 1




# 1 "./include/linux/bits.h" 1





# 1 "./include/vdso/bits.h" 1
# 7 "./include/linux/bits.h" 2
# 1 "./arch/windows/include/generated/uapi/asm/bitsperlong.h" 1
# 8 "./include/linux/bits.h" 2
# 6 "./arch/windows/include/asm/msr-index.h" 2
# 13 "./arch/windows/include/asm/nospec-branch.h" 2
# 1 "./arch/windows/include/asm/unwind_hints.h" 1





# 1 "./arch/windows/include/asm/orc_types.h" 1
# 7 "./arch/windows/include/asm/unwind_hints.h" 2



.macro UNWIND_HINT_END_OF_STACK
 UNWIND_HINT type=1
.endm

.macro UNWIND_HINT_UNDEFINED
 UNWIND_HINT type=0
.endm

.macro UNWIND_HINT_ENTRY
 VALIDATE_UNRET_BEGIN
 UNWIND_HINT_END_OF_STACK
.endm

.macro UNWIND_HINT_REGS base=%rsp offset=0 indirect=0 extra=1 partial=0 signal=1
 .if \base == %rsp
  .if \indirect
   .set sp_reg, 9
  .else
   .set sp_reg, 5
  .endif
 .elseif \base == %rbp
  .set sp_reg, 4
 .elseif \base == %rdi
  .set sp_reg, 3
 .elseif \base == %rdx
  .set sp_reg, 2
 .elseif \base == %r10
  .set sp_reg, 6
 .else
  .error "UNWIND_HINT_REGS: bad base register"
 .endif

 .set sp_offset, \offset

 .if \partial
  .set type, 4
 .elseif \extra == 0
  .set type, 4
  .set sp_offset, \offset + (16*8)
 .else
  .set type, 3
 .endif

 UNWIND_HINT sp_reg=sp_reg sp_offset=sp_offset type=type signal=\signal
.endm

.macro UNWIND_HINT_IRET_REGS base=%rsp offset=0 signal=1
 UNWIND_HINT_REGS base=\base offset=\offset partial=1 signal=\signal
.endm

.macro UNWIND_HINT_IRET_ENTRY base=%rsp offset=0 signal=1
 VALIDATE_UNRET_BEGIN
 UNWIND_HINT_IRET_REGS base=\base offset=\offset signal=\signal
.endm

.macro UNWIND_HINT_FUNC
 UNWIND_HINT sp_reg=5 sp_offset=8 type=5
.endm

.macro UNWIND_HINT_SAVE
 UNWIND_HINT type=6
.endm

.macro UNWIND_HINT_RESTORE
 UNWIND_HINT type=7
.endm
# 14 "./arch/windows/include/asm/nospec-branch.h" 2

# 1 "./arch/windows/include/asm/current.h" 1
# 16 "./arch/windows/include/asm/nospec-branch.h" 2
# 196 "./arch/windows/include/asm/nospec-branch.h"
.macro ANNOTATE_RETPOLINE_SAFE
.Lhere_\@:
 .pushsection .discard.retpoline_safe
 .long .Lhere_\@
 .popsection
.endm
# 213 "./arch/windows/include/asm/nospec-branch.h"
.macro VALIDATE_UNRET_END





.endm






.macro __CS_PREFIX reg:req
 .irp rs,r8,r9,r10,r11,r12,r13,r14,r15
 .ifc \reg,\rs
 .byte 0x2e
 .endif
 .endr
.endm
# 243 "./arch/windows/include/asm/nospec-branch.h"
.macro JMP_NOSPEC reg:req




 jmp *%\reg
 int3

.endm

.macro CALL_NOSPEC reg:req




 call *%\reg

.endm





.macro FILL_RETURN_BUFFER reg:req nr:req ftr:req ftr2=(((1 << 0) << 16) | (( 3*32+21)))
 ALTERNATIVE_2 "jmp .Lskip_rsb_\@", ".rept \nr; ; call 772f; int3; 772:; .endr; add $(32/8) * \nr, %esp;", \ftr, "nop;nop;; call 772f; int3; 772: add $(32/8), %esp; lfence;", \ftr2



.Lskip_rsb_\@:
.endm
# 291 "./arch/windows/include/asm/nospec-branch.h"
.macro __UNTRAIN_RET ibpb_feature, call_depth_insns







.endm
# 311 "./arch/windows/include/asm/nospec-branch.h"
.macro CALL_DEPTH_ACCOUNT




.endm
# 50 "arch/windows/entry/entry_32.S" 2

# 1 "arch/windows/entry/calling.h" 1




# 1 "./arch/windows/include/asm/page_types.h" 1
# 43 "./arch/windows/include/asm/page_types.h"
# 1 "./arch/windows/include/asm/page_32_types.h" 1
# 44 "./arch/windows/include/asm/page_types.h" 2
# 6 "arch/windows/entry/calling.h" 2



# 1 "./arch/windows/include/uapi/asm/ptrace-abi.h" 1
# 10 "arch/windows/entry/calling.h" 2
# 1 "./arch/windows/include/asm/msr.h" 1




# 1 "./arch/windows/include/asm/msr-index.h" 1
# 6 "./arch/windows/include/asm/msr.h" 2
# 11 "arch/windows/entry/calling.h" 2
# 383 "arch/windows/entry/calling.h"
.macro STACKLEAK_ERASE



.endm
# 418 "arch/windows/entry/calling.h"
.macro GET_PERCPU_BASE reg:req
 movq pcpu_unit_offsets(%rip), \reg
.endm
# 52 "arch/windows/entry/entry_32.S" 2
# 62 "arch/windows/entry/entry_32.S"
 .section .entry.text, "ax"
# 82 "arch/windows/entry/entry_32.S"
.macro SWITCH_TO_USER_CR3 scratch_reg:req


 movl %cr3, \scratch_reg
 orl $(1 << 12), \scratch_reg
 movl \scratch_reg, %cr3
.Lend_\@:
.endm


.macro BUG_IF_WRONG_CR3 no_user_check=0
# 108 "arch/windows/entry/entry_32.S"
.endm






.macro SWITCH_TO_KERNEL_CR3 scratch_reg:req

 movl %cr3, \scratch_reg

 testl $(1 << 12), \scratch_reg
 jz .Lend_\@
 andl $(~(1 << 12)), \scratch_reg
 movl \scratch_reg, %cr3

 orl $(1 << 12), \scratch_reg
.Lend_\@:
.endm







.macro FIXUP_FRAME




 andl $0x0000ffff, 4*4(%esp)





 testl $0x2, 4*4(%esp)
 jnz .Lfrom_usermode_no_fixup_\@

 orl $(1 << 29), 4*4(%esp)
# 182 "arch/windows/entry/entry_32.S"
 pushl %ss # ss
 pushl %esp # sp (points at ss)
 addl $7*4, (%esp) # point sp back at the previous context
 pushl 7*4(%esp) # flags
 pushl 7*4(%esp) # cs
 pushl 7*4(%esp) # ip
 pushl 7*4(%esp) # orig_eax
 pushl 7*4(%esp) # gs / function
 pushl 7*4(%esp) # fs
.Lfrom_usermode_no_fixup_\@:
.endm


.macro IRET_FRAME







 testl $(1 << 29), 1*4(%esp)
 jz .Lfinished_frame_\@






 pushl %eax
 pushl %ecx
 movl 5*4(%esp), %eax # (modified) regs->sp

 movl 4*4(%esp), %ecx # flags
 movl %ecx, %ss:-1*4(%eax)

 movl 3*4(%esp), %ecx # cs
 andl $0x0000ffff, %ecx
 movl %ecx, %ss:-2*4(%eax)

 movl 2*4(%esp), %ecx # ip
 movl %ecx, %ss:-3*4(%eax)

 movl 1*4(%esp), %ecx # eax
 movl %ecx, %ss:-4*4(%eax)

 popl %ecx
 lea -4*4(%eax), %esp
 popl %eax
.Lfinished_frame_\@:
.endm


.macro SAVE_ALL pt_regs_ax=%eax switch_stacks=0 skip_gs=0 unwind_espfix=0
 cld
.if \skip_gs == 0
 pushl $0
.endif
 pushl %fs

 pushl %eax
 movl $(0x30), %eax
 movl %eax, %fs
.if \unwind_espfix > 0
 UNWIND_ESPFIX_STACK
.endif
 popl %eax

 FIXUP_FRAME
 pushl %es
 pushl %ds
 pushl \pt_regs_ax
 pushl %ebp
 pushl %edi
 pushl %esi
 pushl %edx
 pushl %ecx
 pushl %ebx
 movl $(0x23), %edx
 movl %edx, %ds
 movl %edx, %es

.if \switch_stacks > 0
 SWITCH_TO_KERNEL_STACK
.endif
.endm
# 288 "arch/windows/entry/entry_32.S"
.macro RESTORE_INT_REGS
 popl %ebx
 popl %ecx
 popl %edx
 popl %esi
 popl %edi
 popl %ebp
 popl %eax
.endm

.macro RESTORE_REGS pop=0
 RESTORE_INT_REGS
1: popl %ds
2: popl %es
3: popl %fs
4: addl $(4 + \pop), %esp
 IRET_FRAME
# 315 "arch/windows/entry/entry_32.S"
.endm
# 341 "arch/windows/entry/entry_32.S"
.macro CHECK_AND_APPLY_ESPFIX
# 389 "arch/windows/entry/entry_32.S"
.endm
# 403 "arch/windows/entry/entry_32.S"
.macro SWITCH_TO_KERNEL_STACK

 BUG_IF_WRONG_CR3

 SWITCH_TO_KERNEL_CR3 scratch_reg=%eax
# 424 "arch/windows/entry/entry_32.S"
 movl %esp, %esi
 movl %esi, %edi


 andl $((~(4096 -1))), %edi
 addl $((4096)), %edi


 movl 42(%edi), %edi







 movl 0x34(%esp), %ecx
 andl $0x3, %ecx

 cmpl $0x3, %ecx
 jb .Lentry_from_kernel_\@


 movl $0x44, %ecx
# 460 "arch/windows/entry/entry_32.S"
.Lcopy_pt_regs_\@:


 subl %ecx, %edi


 movl %edi, %esp





 shrl $2, %ecx
 cld
 rep movsl

 jmp .Lend_\@

.Lentry_from_kernel_\@:
# 509 "arch/windows/entry/entry_32.S"
 movl %esi, %ecx


 andl $((~(4096 -1))), %ecx
 addl $((4096)), %ecx


 sub %esi, %ecx


 orl $(1 << 31), 0x34(%esp)





 testl $(1 << 12), %eax
 jz .Lcopy_pt_regs_\@
 orl $(1 << 30), 0x34(%esp)






 jmp .Lcopy_pt_regs_\@

.Lend_\@:
.endm
# 553 "arch/windows/entry/entry_32.S"
.macro SWITCH_TO_ENTRY_STACK


 movl $0x44, %ecx
# 569 "arch/windows/entry/entry_32.S"
 movl cpu_tss_rw + TSS_sp0, %edi
 subl %ecx, %edi
 movl %esp, %esi


 movl %edi, %ebx


 shrl $2, %ecx
 cld
 rep movsl






 movl %ebx, %esp

.Lend_\@:
.endm
# 598 "arch/windows/entry/entry_32.S"
.macro PARANOID_EXIT_TO_KERNEL_MODE






 testl $(1 << 31), 0x34(%esp)
 jz .Lend_\@




 andl $(~(1 << 31)), 0x34(%esp)


 movl %esp, %esi
 movl cpu_tss_rw + TSS_sp0, %edi


 movl cpu_tss_rw + TSS_sp1, %ecx
 subl %esi, %ecx


 subl %ecx, %edi






 movl %edi, %ebx


 shrl $2, %ecx
 cld
 rep movsl


 movl %ebx, %esp





 testl $(1 << 30), 0x34(%esp)
 jz .Lend_\@


 andl $(~(1 << 30)), 0x34(%esp)

 SWITCH_TO_USER_CR3 scratch_reg=%eax

.Lend_\@:
.endm
# 661 "arch/windows/entry/entry_32.S"
.macro idtentry vector asmsym cfunc has_error_code:req
.globl \asmsym ; .balign 0, 0x90; ; \asmsym:

 cld

 .if \has_error_code == 0
  pushl $0
 .endif


 pushl $\cfunc

 jmp handle_exception
.def \asmsym ; .type $1 ; .endef
.endm

.macro idtentry_irq vector cfunc

 ; .balign 0, 0x90; ; asm_\cfunc:

 SAVE_ALL switch_stacks=1
 ENCODE_FRAME_POINTER
 movl %esp, %eax
 movl PT_ORIG_EAX(%esp), %edx
 movl $-1, PT_ORIG_EAX(%esp)
 call \cfunc
 jmp handle_exception_return
.def asm_\cfunc ; .type $1 ; .endef
.endm

.macro idtentry_sysvec vector cfunc
 idtentry \vector asm_\cfunc \cfunc has_error_code=0
.endm






 .align 16
 .globl __irqentry_text_start
__irqentry_text_start:

# 1 "./arch/windows/include/asm/idtentry.h" 1
# 497 "./arch/windows/include/asm/idtentry.h"
 .align (8 * (1 + 0))
.globl irq_entries_start ; .balign 0, 0x90; ; irq_entries_start:
    vector=0x20
    .rept (256 - 0x20)

0 :

 .byte 0x6a, vector
 jmp asm_common_interrupt

 .fill 0b + (8 * (1 + 0)) - ., 1, 0xcc
 vector = vector+1
    .endr
.def irq_entries_start ; .type $1 ; .endef
# 548 "./arch/windows/include/asm/idtentry.h"
idtentry 0 asm_exc_divide_error exc_divide_error has_error_code=0;
idtentry 4 asm_exc_overflow exc_overflow has_error_code=0;
idtentry 5 asm_exc_bounds exc_bounds has_error_code=0;
idtentry 7 asm_exc_device_not_available exc_device_not_available has_error_code=0;
idtentry 9 asm_exc_coproc_segment_overrun exc_coproc_segment_overrun has_error_code=0;
idtentry 15 asm_exc_spurious_interrupt_bug exc_spurious_interrupt_bug has_error_code=0;
idtentry 16 asm_exc_coprocessor_error exc_coprocessor_error has_error_code=0;
idtentry 19 asm_exc_simd_coprocessor_error exc_simd_coprocessor_error has_error_code=0;


;


idtentry 10 asm_exc_invalid_tss exc_invalid_tss has_error_code=1;
idtentry 11 asm_exc_segment_not_present exc_segment_not_present has_error_code=1;
idtentry 12 asm_exc_stack_segment exc_stack_segment has_error_code=1;
idtentry 13 asm_exc_general_protection exc_general_protection has_error_code=1;
idtentry 17 asm_exc_alignment_check exc_alignment_check has_error_code=1;


idtentry 6 asm_exc_invalid_op exc_invalid_op has_error_code=0;
idtentry 3 asm_exc_int3 exc_int3 has_error_code=0;
idtentry 14 asm_exc_page_fault exc_page_fault has_error_code=1;
# 595 "./arch/windows/include/asm/idtentry.h"
;
# 604 "./arch/windows/include/asm/idtentry.h"
idtentry 1 asm_exc_debug exc_debug has_error_code=0;






;
# 636 "./arch/windows/include/asm/idtentry.h"
idtentry_irq 0xFFFF common_interrupt;
# 705 "arch/windows/entry/entry_32.S" 2

 .align 16
 .globl __irqentry_text_end
__irqentry_text_end:
# 982 "arch/windows/entry/entry_32.S"
.globl entry_INT80_32 ; .balign 0, 0x90; ; entry_INT80_32:

.globl _entry_INT80_32
_entry_INT80_32:

 pushl %eax


 SAVE_ALL pt_regs_ax=$-38

 pushl %esp
 call _do_int80_syscall_32
 add $4, %esp
# 1013 "arch/windows/entry/entry_32.S"
 RESTORE_REGS pop=4 # skip orig_eax/error_code
.Lirq_return:





 iret
# 1046 "arch/windows/entry/entry_32.S"
.macro FIXUP_ESPFIX_STACK
# 1078 "arch/windows/entry/entry_32.S"
.endm

.macro UNWIND_ESPFIX_STACK
# 1091 "arch/windows/entry/entry_32.S"
.endm
