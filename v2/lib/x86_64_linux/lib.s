print:
    movq $1, %rdi
    movq stack_ptr, %rax
    subq $8, %rax
    movq %rax, stack_ptr
    movq (%rax), %rsi
    movq $4, %rdx
    movq $1, %rax
    syscall
    ret
