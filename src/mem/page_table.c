/* page_table.c - helpers para tabelas de página x86_64 (4KiB pages)
 * Usa phys_alloc_page() para alocar frames para page tables intermediárias.
 */

#include "page_table.h"
#include "phys_alloc.h"  /* para phys_alloc_page/free */
#include <stdint.h>

extern void println(const char* str);
extern void print(const char* str);
extern void print_hex_u64(uint64_t v);

/* Aloca uma nova page table (frame físico zerado) */
static uint64_t* alloc_pt(void) {
    uintptr_t paddr = phys_alloc_page();
    if (!paddr) return 0;
    uint64_t *vaddr = (uint64_t*)(uintptr_t)paddr;  /* assume identity map para kernel */
    for (int i = 0; i < 512; i++) vaddr[i] = 0;
    return vaddr;
}

/* Libera uma page table */
static void free_pt(uint64_t *pt) {
    if (pt) phys_free_page((uintptr_t)pt);
}

/* Índices para níveis de page table */
#define PML4_INDEX(v) (((v) >> 39) & 0x1FF)
#define PDP_INDEX(v)  (((v) >> 30) & 0x1FF)
#define PD_INDEX(v)   (((v) >> 21) & 0x1FF)
#define PT_INDEX(v)   (((v) >> 12) & 0x1FF)

int map_page(uint64_t *pml4, uintptr_t vaddr, uintptr_t paddr, uint64_t flags) {
    if (!pml4) return -1;

    uint64_t pml4e = pml4[PML4_INDEX(vaddr)];
    uint64_t *pdp;
    if (!(pml4e & PAGE_PRESENT)) {
        pdp = alloc_pt();
        if (!pdp) return -1;
        pml4[PML4_INDEX(vaddr)] = (uintptr_t)pdp | PAGE_PRESENT | PAGE_WRITE;
    } else {
        pdp = (uint64_t*)(uintptr_t)(pml4e & ~0xFFFULL);
    }

    uint64_t pdpe = pdp[PDP_INDEX(vaddr)];
    uint64_t *pd;
    if (!(pdpe & PAGE_PRESENT)) {
        pd = alloc_pt();
        if (!pd) return -1;
        pdp[PDP_INDEX(vaddr)] = (uintptr_t)pd | PAGE_PRESENT | PAGE_WRITE;
    } else {
        pd = (uint64_t*)(uintptr_t)(pdpe & ~0xFFFULL);
    }

    uint64_t pde = pd[PD_INDEX(vaddr)];
    uint64_t *pt;
    if (!(pde & PAGE_PRESENT)) {
        pt = alloc_pt();
        if (!pt) return -1;
        pd[PD_INDEX(vaddr)] = (uintptr_t)pt | PAGE_PRESENT | PAGE_WRITE;
    } else {
        pt = (uint64_t*)(uintptr_t)(pde & ~0xFFFULL);
    }

    pt[PT_INDEX(vaddr)] = (paddr & ~0xFFFULL) | flags;
    return 0;
}

int unmap_page(uint64_t *pml4, uintptr_t vaddr) {
    if (!pml4) return -1;

    uint64_t pml4e = pml4[PML4_INDEX(vaddr)];
    if (!(pml4e & PAGE_PRESENT)) return -1;
    uint64_t *pdp = (uint64_t*)(uintptr_t)(pml4e & ~0xFFFULL);

    uint64_t pdpe = pdp[PDP_INDEX(vaddr)];
    if (!(pdpe & PAGE_PRESENT)) return -1;
    uint64_t *pd = (uint64_t*)(uintptr_t)(pdpe & ~0xFFFULL);

    uint64_t pde = pd[PD_INDEX(vaddr)];
    if (!(pde & PAGE_PRESENT)) return -1;
    uint64_t *pt = (uint64_t*)(uintptr_t)(pde & ~0xFFFULL);

    pt[PT_INDEX(vaddr)] = 0;  /* desmapear */
    /* TODO: liberar PT/PD/PDP se vazios (por simplicidade, não feito) */
    return 0;
}

uintptr_t vaddr_to_paddr(uint64_t *pml4, uintptr_t vaddr) {
    if (!pml4) return 0;

    uint64_t pml4e = pml4[PML4_INDEX(vaddr)];
    if (!(pml4e & PAGE_PRESENT)) return 0;
    uint64_t *pdp = (uint64_t*)(uintptr_t)(pml4e & ~0xFFFULL);

    uint64_t pdpe = pdp[PDP_INDEX(vaddr)];
    if (!(pdpe & PAGE_PRESENT)) return 0;
    uint64_t *pd = (uint64_t*)(uintptr_t)(pdpe & ~0xFFFULL);

    uint64_t pde = pd[PD_INDEX(vaddr)];
    if (!(pde & PAGE_PRESENT)) return 0;
    uint64_t *pt = (uint64_t*)(uintptr_t)(pde & ~0xFFFULL);

    uint64_t pte = pt[PT_INDEX(vaddr)];
    if (!(pte & PAGE_PRESENT)) return 0;
    return (pte & ~0xFFFULL) | (vaddr & 0xFFFULL);
}