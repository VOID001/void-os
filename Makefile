TARGET=i686-elf
CC=$(TARGET)-gcc
LD=$(TARGET)-ld
AS=$(TARGET)-as
CFLAGS=-ffreestanding -O0 -nostdlib -std=gnu99 -Wall -Wextra
LDFLAGS=-lgcc -T linker.ld

void-os.iso: kernel.bin grub.cfg
	- mkdir -p isodir/boot/grub
	- cp kernel.bin isodir/boot/kernel.bin
	- cp grub.cfg isodir/boot/grub/grub.cfg
	- grub-mkrescue -o void-os.iso isodir

boot-qemu: void-os.iso
	- qemu-system-i386 -cdrom void-os.iso

.o: .c
	$(CC) $(CFLAGS) -c $<

kernel.bin: kernel.o boot.o
	$(CC) $(CFLAGS) -o kernel.bin $(LDFLAGS) boot.o kernel.o

.PHONY: clean boot-qemu

clean:
	- rm -rf isodir
	- rm -rf *.o
	- rm -rf *.bin
	- rm -rf *.iso
