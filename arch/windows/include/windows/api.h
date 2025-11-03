#ifndef WINDOWSAPI_H
#define WINDOWSAPI_H

struct page;

void *win_allocate_memory(unsigned long size);

void win_add_page(struct page *page);
void win_del_page(struct page *page);

/* page virtual address to struct page */
struct page *win_virt_to_page(const void *vaddr);
void *win_page_to_virt(const struct page *page);

#endif
