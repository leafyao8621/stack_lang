    .section .data
buf:
    .fill 100
int_buf:
    .fill 100
stack:
    .fill 800
stack_ptr:
    .quad 0
strlen:
    .quad 0
int0:
    .quad 0
    .section .text
    .globl _start
    .include "lib.s"
_start:
    movabsq $stack, %rax
    movq %rax, stack_ptr

    movq stack_ptr, %rax
    movabsq $int0, %rbx
    movq %rbx, (%rax)
    addq $8, %rax
    movq %rax, stack_ptr
    call input

    movq stack_ptr, %rax
    movq int0, %rbx
    movq %rbx, (%rax)
    addq $8, %rax
    movq %rax, stack_ptr
    call println_int

    movq $60, %rax
    movq $0, %rdi
    syscall

