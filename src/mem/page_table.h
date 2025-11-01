/* page_table.h - helpers para manipulação de tabelas de página x86_64 */
#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <stdint.h>

/* Flags para entradas de página (x86_64) */
#define PAGE_PRESENT    (1ULL << 0)
#define PAGE_WRITE      (1ULL << 1)
#define PAGE_USER       (1ULL << 2)
#define PAGE_NX         (1ULL << 63)  /* No Execute */

/* Mapeia uma página 4KiB virtual para física.
 * Aloca page tables intermediárias se necessário usando phys_alloc_page().
 * Retorna 0 em sucesso, -1 em erro.
 */
int map_page(uint64_t *pml4, uintptr_t vaddr, uintptr_t paddr, uint64_t flags);

/* Desmapeia uma página 4KiB virtual.
 * Libera page tables intermediárias se vazias (opcional, por simplicidade não implementado).
 */
int unmap_page(uint64_t *pml4, uintptr_t vaddr);

/* Traduz endereço virtual para físico usando o PML4.
 * Retorna paddr ou 0 se não mapeado.
 */
uintptr_t vaddr_to_paddr(uint64_t *pml4, uintptr_t vaddr);

#endif /* PAGE_TABLE_H */