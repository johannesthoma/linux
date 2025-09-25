/* no ELF on windows ... */
#define ELF_CLASS 0

typedef unsigned long elf_greg_t;
typedef unsigned long elf_fpreg_t; /* TODO: probably float or double ... */

/* wrong... */
#define ELF_NGREG 1
typedef elf_greg_t elf_gregset_t[ELF_NGREG];
typedef elf_fpreg_t elf_fpregset_t[ELF_NGREG];

