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
print_int:
    movabsq $1, %rdi
    movq stack_ptr, %rax
    subq $8, %rax
    movq %rax, stack_ptr
    movq (%rax), %rax
    movabsq $int_buf, %r10
    movabsq $0, %r11
    movabsq $0, %r12
    movabsq $10, %rbx
print_int_loop0:
    divq %rbx
    addq $48, %rdx
    movb %dl, (%r10)
    movabsq $0, %rdx
    addq $1, %r10
    addq $1, %r11
    cmpq %rax, %r12
    jne print_int_loop0
    movq %r11, strlen
    movabsq $buf, %r12
    movabsq $0, %r14
print_int_loop1:
    subq $1, %r10
    movb (%r10), %r13b
    movb %r13b, (%r12)
    addq $1, %r12
    subq $1, %r11
    cmpq %r11, %r14
    jne print_int_loop1
    movabsq $buf, %rsi
    movq strlen, %rdx
    movabsq $1, %rax
    syscall
    ret
