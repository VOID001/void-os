OBJS=kernel.o 

TARGET=i686-elf
CC=$(TARGET)-gcc
LD=$(TARGET)-ld
AS=$(TARGET)-as
CFLAGS=-ffreestanding -O0 -nostdlib -std=gnu99 -Wall -Wextra
LDFLAGS=-lgcc -T linker.ld

CRTI_OBJ=crti.o
CRTBEGIN_OBJ=$(shell $(CC) $(CLFAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ=$(shell $(CC) $(CLFAGS) -print-file-name=crtend.o)
CRTN_OBJ=crtn.o

OBJ_LINK_LIST=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)

void-os.iso: kernel.bin grub.cfg
	- mkdir -p isodir/boot/grub
	- cp kernel.bin isodir/boot/kernel.bin
	- cp grub.cfg isodir/boot/grub/grub.cfg
	- grub-mkrescue -o void-os.iso isodir

boot-qemu: void-os.iso
	- qemu-system-i386 -cdrom void-os.iso

.o: .c
	- $(CC) $(CFLAGS) -c $<

crti.o: crti.s
	- $(AS) $< -o $@

crtn.o: crtn.s
	- $(AS) $< -o $@

kernel.bin: boot.o $(OBJ_LINK_LIST)
	- $(CC) $(CFLAGS) -o kernel.bin $(LDFLAGS) boot.o $(OBJ_LINK_LIST)

.PHONY: clean boot-qemu

clean:
	- rm -rf isodir
	- rm -rf *.o
	- rm -rf *.bin
	- rm -rf *.iso
