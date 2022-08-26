    .data
stack: .fill 100, 8, 0
stack_ptr: .quad 0
strlen: .quad 0
str_buf: .fill 100
int_buf: .fill 100
int0: .quad 0
    .text
    .global _start
    .include "lib.s"
_start:
    ldr x9, =stack
    ldr x10, =stack_ptr
    str x9, [x10]



    ldr x9, =stack_ptr
    ldr x10, [x9]
    ldr x11, =int0
    str x11, [x10]
    add x10, x10, #8
    str x10, [x9]
    stp x29, x30, [sp, #-16]!
    bl input
    ldp x29, x30, [sp], #16

    ldr x9, =stack_ptr
    ldr x10, [x9]
    ldr x11, =int0
    ldr x11, [x11]
    str x11, [x10]
    add x10, x10, #8
    str x10, [x9]
    stp x29, x30, [sp, #-16]!
    bl println_int
    ldp x29, x30, [sp], #16

    mov x8, #93
    mov x0, #0
    svc #0
