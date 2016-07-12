# Declare constants for multiboot header( Used by grub)
.set ALIGN, 1 << 0
.set MEMINFO, 1 << 1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

# Decalre a multiboot header, marks the program as a kernel
# These are magic values documented in multiboot standard
# bootloader will serach for this signature in the first 8KiB
# of the kernel file, we use linker script to force the header
# section to be within the first 8KiB of kernel file

.section .multiboot
.align   4
.long    MAGIC
.long    FLAGS
.long    CHECKSUM

# Set up a stack for the kernel, since grub does not provide
# The setup of %esp, We manually create a small stack by
# creating a symbol at the bottom of it, then allocating 16384
# bytes for it, and finally creating a symbol at the top. The stack grows
# downwards on x86. The stack is in its own section so it can be marked nobits
# which means the kernel file is smaller because it does not contain an
# uninitilaized stack

.section .bootstrap_stack, "aw", @nobits

stack_bottom:
	.skip 16384 # 16KiB

stack_top:

# The linker script specifies _start as the entry point to the kernel
# and the bootloader will jump to this position once the kernel has been
# loaded It doesn't make sense to return from this function as the bootloader
# is gone

.section .text
.global  _start
.type    _start, @function

_start:

    # The bootloader loaded us into 32-bit protected mode on a x86 machine
    # Interrupts, Paging is disabled, The process state is as defined in
    # the multiboot standard. The kernel has full control of the CPU.
    # The kernel can only make use of hardware features and any code provides
    # as part of itself. no <stdio.h> no printf, etc... Now we have the full
    # access to CPU
    
    # Setup the stack, it grow downwards on x86 machines, so make esp point
    # to stack_top(higher address)
    
    mov $stack_top, %esp
    
    # Here we should initialize crucial processor state before the high level
    # kernel is entered. It's best to minimize the early environment where
    # crucial features are offline. Note now the processor is not fully initialized
    # yet: Features such as floating point instructions and instruction set extensions
    # not init yet, GDT should be loaded here, Paging should be enabled here.
    # C++ features such as global constructors and exceptions will require runtime support
    # to work as well

    # Here we init the GDT
    # We construct the GDT descriptor right below the stack-top address(lower than it)
    # 6 bytes just fits the GDT descriptor size
    # gdt_size - 1 and gdt_addr

    subl $6, %esp
    movw gdt_size_minus_one, %cx
    movw %cx, 0(%esp)
    movl $gdt, %ecx
    movl %ecx, 2(%esp)
    lgdt 0(%esp)
    addl $6, %esp

    # Switch CS 
    push $0x08
    push $1f        # Push the start address of the code marked as tag 1
    retf
    
    # Enter the high-level kernel
1:    
    movw $0x10, %cx
    movw %cx, %ds
    movw %cx, %es
    movw %cx, %fs
    movw %cx, %gs
    movw %cx, %ss
    
haha:    ljmp $0x08, $haha2
haha2:  ljmp $0x08, $haha3
haha3:  ljmp $0x08, $haha4
haha4:

    call _init

    call kernel_main

    call _fini

    cli
    
    # Halt the system forever after kernel return
.Lhlt:
    hlt
    jmp .Lhlt
    
    # Set the size of the _start symbol to the current location '.' minus its start.
    # This is useful when debugging or when implementing call tracing.
.size _start, . - _start
