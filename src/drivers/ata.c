/* ata.c - driver ATA/IDE básico para disco */
#include "ata.h"
#include "io.h"

extern void println(const char* str);

/* Portas I/O do controlador ATA primário */
#define ATA_PRIMARY_DATA       0x1F0
#define ATA_PRIMARY_ERROR      0x1F1
#define ATA_PRIMARY_SECCOUNT   0x1F2
#define ATA_PRIMARY_LBA_LO     0x1F3
#define ATA_PRIMARY_LBA_MID    0x1F4
#define ATA_PRIMARY_LBA_HI     0x1F5
#define ATA_PRIMARY_DRIVE      0x1F6
#define ATA_PRIMARY_STATUS     0x1F7
#define ATA_PRIMARY_COMMAND    0x1F7

/* Comandos ATA */
#define ATA_CMD_READ_SECTORS   0x20
#define ATA_CMD_WRITE_SECTORS  0x30

/* Status bits */
#define ATA_STATUS_BSY         0x80
#define ATA_STATUS_DRQ         0x08
#define ATA_STATUS_ERR         0x01

/* Delay curto */
static void ata_delay(void) {
    for (volatile int i = 0; i < 1000; i++);
}

/* Espera até disco não estar ocupado */
static int ata_wait_not_busy(void) {
    ata_delay();
    for (int i = 0; i < 10000; i++) {
        uint8_t status = inb(ATA_PRIMARY_STATUS);
        if (!(status & ATA_STATUS_BSY)) return 0;
        ata_delay();
    }
    return -1; /* timeout */
}

/* Espera até dados prontos */
static int ata_wait_drq(void) {
    ata_delay();
    for (int i = 0; i < 10000; i++) {
        uint8_t status = inb(ATA_PRIMARY_STATUS);
        if (status & ATA_STATUS_ERR) return -2; /* erro */
        if (status & ATA_STATUS_DRQ) return 0;  /* pronto */
        if (!(status & ATA_STATUS_BSY)) ata_delay();
    }
    return -1; /* timeout */
}

void ata_init(void) {
    /* Selecionar drive master (0xA0 = master, 0xB0 = slave) */
    outb(ATA_PRIMARY_DRIVE, 0xA0);
    ata_delay();
    
    /* Ler status alternativo 4x para delay */
    for (int i = 0; i < 4; i++) {
        inb(ATA_PRIMARY_STATUS);
    }
}

int ata_read_sector(uint32_t sector, uint8_t *buffer) {
    if (!buffer) return -1;
    
    /* Aguardar disco livre */
    if (ata_wait_not_busy() < 0) {
        println("ata: timeout esperando disco livre");
        return -1;
    }
    
    /* Selecionar drive master + LBA mode + bits 24-27 do LBA */
    outb(ATA_PRIMARY_DRIVE, 0xE0 | ((sector >> 24) & 0x0F));
    ata_delay();
    
    /* Configurar setor */
    outb(ATA_PRIMARY_SECCOUNT, 1);
    outb(ATA_PRIMARY_LBA_LO, sector & 0xFF);
    outb(ATA_PRIMARY_LBA_MID, (sector >> 8) & 0xFF);
    outb(ATA_PRIMARY_LBA_HI, (sector >> 16) & 0xFF);
    
    /* Enviar comando de leitura */
    outb(ATA_PRIMARY_COMMAND, ATA_CMD_READ_SECTORS);
    
    /* Aguardar dados prontos */
    int drq_result = ata_wait_drq();
    if (drq_result == -2) {
        println("ata: disco reportou erro");
        uint8_t err = inb(ATA_PRIMARY_ERROR);
        (void)err; /* evitar warning */
        return -1;
    }
    if (drq_result < 0) {
        println("ata: timeout aguardando dados");
        return -1;
    }
    
    /* Ler 256 palavras (512 bytes) */
    uint16_t *buf16 = (uint16_t*)buffer;
    for (int i = 0; i < 256; i++) {
        buf16[i] = inw(ATA_PRIMARY_DATA);
    }
    
    return 0;
}

int ata_write_sector(uint32_t sector, const uint8_t *buffer) {
    if (!buffer) return -1;
    
    /* Aguardar disco livre */
    if (ata_wait_not_busy() < 0) {
        println("ata: timeout esperando disco livre");
        return -1;
    }
    
    /* Selecionar drive master + LBA mode */
    outb(ATA_PRIMARY_DRIVE, 0xE0 | ((sector >> 24) & 0x0F));
    outb(ATA_PRIMARY_SECCOUNT, 1);
    outb(ATA_PRIMARY_LBA_LO, sector & 0xFF);
    outb(ATA_PRIMARY_LBA_MID, (sector >> 8) & 0xFF);
    outb(ATA_PRIMARY_LBA_HI, (sector >> 16) & 0xFF);
    outb(ATA_PRIMARY_COMMAND, ATA_CMD_WRITE_SECTORS);
    
    /* Aguardar dados prontos */
    if (ata_wait_drq() < 0) {
        println("ata: erro aguardando disco pronto para escrita");
        return -1;
    }
    
    /* Escrever 256 palavras (512 bytes) */
    const uint16_t *buf16 = (const uint16_t*)buffer;
    for (int i = 0; i < 256; i++) {
        outw(ATA_PRIMARY_DATA, buf16[i]);
    }
    
    /* Flush cache */
    if (ata_wait_not_busy() < 0) {
        println("ata: timeout no flush");
        return -1;
    }
    
    return 0;
}