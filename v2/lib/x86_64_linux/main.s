    .section .data
buf:
    .fill 1000
stack:
    .fill 1000
stack_ptr:
    .quad 0
    .section .text
    .globl main
    .include "lib.s"
str0:
    .asciz "abc\n"
main:
    movabs $stack, %rax
    movabs %rax, stack_ptr
    movq $str0, (%rax)
    addq $8, (%rax)
    movq %rax, stack_ptr
    call print
