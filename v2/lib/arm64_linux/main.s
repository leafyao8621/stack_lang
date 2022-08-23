    .data
stack: .fill 100, 8, 0
stack_ptr: .quad 0
strlen: .quad 0
str_buf: .fill 100
int_buf: .fill 100
str0: .asciz "abc"
    .text
    .global _start
    .include "lib.s"
_start:
    ldr x9, =stack
    ldr x10, =stack_ptr
    str x9, [x10]

    ldr x9, =stack_ptr
    ldr x10, [x9]
    mov x11, #65
    str x11, [x10]
    add x10, x10, #8
    str x10, [x9]
    bl print_int


    ldr x9, =stack_ptr
    ldr x10, [x9]
    mov x11, #65
    str x11, [x10]
    add x10, x10, #8
    str x10, [x9]
    bl println_int

    mov x8, #93
    mov x0, #0
    svc #0
