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
str0:
    .asciz "abc"
    .section .text
    .globl _start
    .include "lib.s"
_start:
    movabsq $stack, %rax
    movq %rax, stack_ptr

    // movq stack_ptr, %rax
    // movq $str0, (%rax)
    // movabsq $3, %rbx
    // movq %rbx, strlen
    // addq $8, %rax
    // movq %rax, stack_ptr
    // call print_str

    // movq stack_ptr, %rax
    // movabsq $10, %rbx
    // movq %rbx, (%rax)
    // addq $8, %rax
    // movq %rax, stack_ptr
    // call print_chr
    // movq stack_ptr, %rax
    // movabsq $65, %rbx
    // movq %rbx, (%rax)
    // addq $8, %rax
    // movq %rax, stack_ptr
    // call print_chr

    // movq stack_ptr, %rax
    // movabsq $10, %rbx
    // movq %rbx, (%rax)
    // addq $8, %rax
    // movq %rax, stack_ptr
    // call print_chr

    // movq stack_ptr, %rax
    // movq $str0, (%rax)
    // movabsq $3, %rbx
    // movq %rbx, strlen
    // addq $8, %rax
    // movq %rax, stack_ptr
    // call println_str
    // movq stack_ptr, %rax
    // movabsq $97, %rbx
    // movq %rbx, (%rax)
    // addq $8, %rax
    // movq %rax, stack_ptr
    // call println_chr
    
    movq stack_ptr, %rax
    movabsq $-123456, %rbx
    movq %rbx, (%rax)
    addq $8, %rax
    movq %rax, stack_ptr
    call print_int

    movq $60, %rax
    movq $0, %rdi
    syscall

