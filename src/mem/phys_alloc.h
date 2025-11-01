/* phys_alloc.h - alocador físico simples baseado em bitmap */
#ifndef PHYS_ALLOC_H
#define PHYS_ALLOC_H

#include <stdint.h>

/* Inicializa o alocador usando a região [start, end) fornecida pelo bootloader */
void phys_init(uint64_t start, uint64_t end);

/* Aloca uma página física (4KiB). Retorna endereço físico ou 0 se falhar. */
uint64_t phys_alloc_page(void);

/* Libera uma página previamente alocada. */
void phys_free_page(uint64_t paddr);

/* Stats da memória */
uint64_t phys_get_total_pages(void);
uint64_t phys_get_free_pages(void);

#endif /* PHYS_ALLOC_H */
