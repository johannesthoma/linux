#define PAGE_SIZE 4096
#define PAGE_MASK ~(PAGE_SIZE-1)
#define PAGE_SHIFT 12

#define THREAD_SIZE PAGE_SIZE
#define TASK_SIZE 0x80000000

typedef int pgtable_t;
typedef int pgprot_t;
typedef int pgd_t;
