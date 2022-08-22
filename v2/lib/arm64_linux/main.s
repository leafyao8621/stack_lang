    .data
stack: .fill 100, 8, 0
stack_ptr: .quad 0
strlen: .quad 0
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
    ldr x11, =str0
    str x11, [x10]
    add x10, x10, #8
    str x10, [x9]

    ldr x9, =strlen
    mov x10, #3
    str x10, [x9]

    bl print_str

    mov x8, #93
    mov x0, #0
    svc #0
