/* io.h - funções de I/O para portas x86 */
#ifndef IO_H
#define IO_H

#include <stdint.h>

/* Escreve um byte em uma porta I/O */
static inline void outb(uint16_t port, uint8_t val) __attribute__((always_inline));
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port) : "memory");
}

/* Lê um byte de uma porta I/O */
static inline uint8_t inb(uint16_t port) __attribute__((always_inline));
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

/* Lê uma palavra (16 bits) de uma porta I/O */
static inline uint16_t inw(uint16_t port) __attribute__((always_inline));
static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/* Escreve uma palavra (16 bits) em uma porta I/O */
static inline void outw(uint16_t port, uint16_t val) __attribute__((always_inline));
static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

#endif /* IO_H */