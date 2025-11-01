/* ata.h - driver básico ATA/IDE para acesso a disco */
#ifndef ATA_H
#define ATA_H

#include <stdint.h>

/* Inicializa o driver ATA */
void ata_init(void);

/* Lê um setor (512 bytes) do disco primário 
 * sector: número do setor (LBA28)
 * buffer: ponteiro para buffer de 512 bytes
 * Retorna 0 em sucesso, -1 em erro
 */
int ata_read_sector(uint32_t sector, uint8_t *buffer);

/* Escreve um setor (512 bytes) no disco primário 
 * sector: número do setor (LBA28)
 * buffer: ponteiro para buffer de 512 bytes
 * Retorna 0 em sucesso, -1 em erro
 */
int ata_write_sector(uint32_t sector, const uint8_t *buffer);

#endif /* ATA_H */