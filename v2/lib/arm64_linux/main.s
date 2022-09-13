    .data
stack: .fill 100, 8, 0
stack_ptr: .quad 0
strlen: .quad 0
str_buf: .fill 100
int_buf: .fill 100
seed: .quad 0
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
    mov x11, #0
    str x11, [x10]
    add x10, x10, #8
    str x10, [x9]
    stp x29, x30, [sp, #-16]!
    bl srand
    ldp x29, x30, [sp], #16

    mov x22, #0
loop0:
    stp x29, x30, [sp, #-16]!
    bl rand
    ldp x29, x30, [sp], #16
    stp x29, x30, [sp, #-16]!
    bl println_int
    ldp x29, x30, [sp], #16
    add x22, x22, #1
    cmp x22, #10
    blt loop0

    mov x8, #93
    mov x0, #0
    svc #0
