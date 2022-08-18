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
seed:
    .quad 0
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

    movabsq $0, %r9
loop0:
    call rand
    call println_int
    addq $1, %r9
    cmpq $10, %r9
    jl loop0

    movq $60, %rax
    movq $0, %rdi
    syscall

