# makefile do Kernel ne pai
CC = gcc
AS = nasm
LD = ld

CFLAGS = -ffreestanding -m64 -nostdlib -Isrc -Isrc/commands -O2 -fno-stack-protector -fno-pie
ASFLAGS = -f elf64
LDFLAGS = -n -T linker.ld

SRCS = src/boot.asm src/kernel.c src/mem/multiboot.c src/mem/phys_alloc.c src/mem/page_table.c src/commands/commands.c src/commands/cd.c src/commands/ls.c src/commands/echo.c src/commands/help.c src/commands/exit.c
OBJS = src/boot.o src/kernel.o src/mem/multiboot.o src/mem/phys_alloc.o src/mem/page_table.o src/commands/commands.o src/commands/cd.o src/commands/ls.o src/commands/echo.o src/commands/help.o src/commands/exit.o

.PHONY: all clean run

all: kernel.bin

kernel.bin: $(OBJS)
	@echo "Linkando kernel 64-bit..."
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

src/boot.o: src/boot.asm
	@echo "Montando $<..."
	$(AS) $(ASFLAGS) -o $@ $<

src/kernel.o: src/kernel.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

src/mem/multiboot.o: src/mem/multiboot.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

src/mem/phys_alloc.o: src/mem/phys_alloc.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

src/mem/page_table.o: src/mem/page_table.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

src/commands/commands.o: src/commands/commands.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

src/commands/cd.o: src/commands/cd.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

src/commands/ls.o: src/commands/ls.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

src/commands/echo.o: src/commands/echo.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

src/commands/help.o: src/commands/help.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

src/commands/exit.o: src/commands/exit.c
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c -o $@ $<

run: kernel.bin
	@echo "Executando no QEMU..."
	@echo "Dica: Pressione Ctrl+A, depois X para sair"
	qemu-system-x86_64 -kernel kernel.bin -serial stdio -display none -no-reboot

iso: kernel.bin
	@echo "Criando ISO bootável..."
	@mkdir -p iso/boot/grub
	@cp kernel.bin iso/boot/kernel.bin
	@echo "set timeout=0" > iso/boot/grub/grub.cfg
	@echo "set default=0" >> iso/boot/grub/grub.cfg
	@echo "" >> iso/boot/grub/grub.cfg
	@echo "menuentry \"Kernel64 OS\" {" >> iso/boot/grub/grub.cfg
	@echo "    multiboot2 /boot/kernel.bin" >> iso/boot/grub/grub.cfg
	@echo "    boot" >> iso/boot/grub/grub.cfg
	@echo "}" >> iso/boot/grub/grub.cfg
	@grub-mkrescue -o kernel.iso iso/ 2>/dev/null || (echo "ERRO: Instale grub-pc-bin e xorriso" && exit 1)
	@echo "ISO criado: kernel.iso"

run-iso: iso
	@echo "Executando ISO no QEMU..."
	@echo "Dica: Pressione Ctrl+A, depois X para sair"
	qemu-system-x86_64 -cdrom kernel.iso -nographic

clean:
	@echo "Limpando..."
	rm -f $(OBJS) kernel.bin kernel.iso
	rm -rf iso/

build: all

help:
	@echo "Comandos disponíveis:"
	@echo "  make         - Compilar kernel"
	@echo "  make iso     - Criar ISO bootável"
	@echo "  make run-iso - Executar ISO no QEMU"
	@echo "  make clean   - Limpar arquivos"
	@echo ""
	@echo "Nota: 'make run' não funciona no QEMU 6.2+"
	@echo "      Use 'make run-iso' ao invés"