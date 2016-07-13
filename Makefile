OBJS=kernel.o \
	 terminal.o \
	 stdio.o \
	 gdt.o \
	 idt.o \
	 interrupt_handler.o \
	 interrupt.o \
     welcomepage.o \

TARGET=i686-elf
CC=$(TARGET)-gcc
LD=$(TARGET)-ld
AS=$(TARGET)-as
CFLAGS=-ffreestanding -Og -std=gnu99 -Wall -Wextra
LDFLAGS= -T linker.ld -nostdlib -lgcc

CRTI_OBJ=crti.o
CRTBEGIN_OBJ=$(shell $(CC) $(CLFAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ=$(shell $(CC) $(CLFAGS) -print-file-name=crtend.o)
CRTN_OBJ=crtn.o

OBJ_LINK_LIST=$(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)

void-os.iso: kernel.bin grub.cfg
	 mkdir -p isodir/boot/grub
	 cp kernel.bin isodir/boot/kernel.bin
	 cp grub.cfg isodir/boot/grub/grub.cfg
	 grub-mkrescue -o void-os.iso isodir

boot-qemu: void-os.iso
	 qemu-system-i386 -cdrom void-os.iso

boot-qemu-dbg: void-os.iso
	 qemu-system-i386 -cdrom void-os.iso -d int -no-reboot -no-shutdown

boot.o: boot.s
	 $(CC) $(CFLAGS) -c $<

%.o: %.c
	 $(CC) $(CFLAGS) -c $<

%.o: %.s
	 $(CC) -c $<

crti.o: crti.s
	 $(AS) $< -o $@

crtn.o: crtn.s
	 $(AS) $< -o $@

kernel.bin: boot.o $(OBJ_LINK_LIST)
	 $(CC) $(CFLAGS) -o kernel.bin $(LDFLAGS) boot.o $(OBJ_LINK_LIST)

.PHONY: clean boot-qemu boot-qemu-dbg

clean:
	- rm -rf isodir
	- rm -rf *.o
	- rm -rf *.bin
	- rm -rf *.iso
