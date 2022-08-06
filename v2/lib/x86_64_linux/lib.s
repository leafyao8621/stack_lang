print_str:
    movabsq $1, %rdi
    movq stack_ptr, %rax
    subq $8, %rax
    movq %rax, stack_ptr
    movq (%rax), %rsi
    movq strlen, %rdx
    movabsq $1, %rax
    syscall
    ret
print_chr:
    movabsq $1, %rdi
    movq stack_ptr, %rax
    subq $8, %rax
    movq %rax, stack_ptr
    movb (%rax), %bl
    movb %bl, buf
    movabsq $buf, %rsi
    movabsq $1, %rdx
    movabsq $1, %rax
    syscall
    ret
println_str:
    movabsq $1, %rdi
    movq stack_ptr, %rax
    subq $8, %rax
    movq %rax, stack_ptr
    movq (%rax), %rsi
    movq strlen, %rdx
    movabsq $1, %rax
    syscall
    movabsq $1, %rdi
    movabsq $buf, %rsi
    movb $10, buf
    movabsq $1, %rdx
    movabsq $1, %rax
    syscall
    ret
println_chr:
    movabsq $1, %rdi
    movq stack_ptr, %rax
    subq $8, %rax
    movq %rax, stack_ptr
    movb (%rax), %bl
    movb %bl, buf
    movabsq $buf, %rsi
    movabsq $1, %rdx
    movabsq $1, %rax
    syscall
    movabsq $1, %rdi
    movabsq $buf, %rsi
    movb $10, buf
    movabsq $1, %rdx
    movabsq $1, %rax
    syscall
    ret
