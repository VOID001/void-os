.macro PUSH_ALL
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %ebp
    pushl %esi
    pushl %edi
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
.endm

.macro POP_ALL
    popl %gs
    popl %fs
    popl %es
    popl %ds
    popl %edi
    popl %esi
    popl %ebp
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax
.endm



.global int_stub
.type int_stub, @function
int_stub:
    PUSH_ALL
    call handler_stub
    POP_ALL
    iret
.size int_stub, . - int_stub


.global int_21h
.type int_21h, @function
int_21h:
    PUSH_ALL
    call handler_21h
    POP_ALL
    iret
.size int_21h, . - int_21h


.global int_20h
.type int_21h, @function
int_20h:
    PUSH_ALL
    call handler_20h
    POP_ALL
    iret
.size int_20h, . - int_20h
