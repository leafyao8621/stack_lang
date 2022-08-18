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
index:
    .quad 0
mt:
    .fill 624, 8, 0
    .section .text
    .globl _start
    .include "lib.s"
_start:
    movabsq $stack, %rax
    movq %rax, stack_ptr

    movq stack_ptr, %rax
    movq $1000, %rbx
    movq %rbx, (%rax)
    addq $8, %rax
    movq %rax, stack_ptr
    call srand

    movq stack_ptr, %rax
    movq mt, %rbx
    movq %rbx, (%rax)
    addq $8, %rax
    movq %rax, stack_ptr
    call println_int

    movq $60, %rax
    movq $0, %rdi
    syscall

