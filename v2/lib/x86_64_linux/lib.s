calc_strlen:
    movq stack_ptr, %rax
    subq $8, %rax
    movq (%rax), %rbx
    movabsq $0, %rcx
calc_strlen_loop0:
    movb (%rbx), %r10b
    cmpb $0, %r10b
    je calc_strlen_end_loop0
    addq $1, %rbx
    addq $1, %rcx
    jmp calc_strlen_loop0
calc_strlen_end_loop0:
    movq %rcx, strlen
    ret
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
    movabsq $0, %rdx
    movq %rax, %r15
    movq $0, %r10
    cmpq %r10, %r15
    jge print_int_eif0
    negq %rax
print_int_eif0:
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
    movabsq $buf, %r12
    movabsq $0, %r14
    cmpq %r14, %r15
    jge print_int_eif1
    movb $45, (%r12)
    addq $1, %r12
    addq $1, %r11
print_int_eif1:
    movq %r11, strlen
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
println_int:
    movabsq $1, %rdi
    movq stack_ptr, %rax
    subq $8, %rax
    movq %rax, stack_ptr
    movq (%rax), %rax
    movabsq $0, %rdx
    movq %rax, %r15
    movq $0, %r10
    cmpq %r10, %r15
    jge println_int_eif0
    negq %rax
println_int_eif0:
    movabsq $int_buf, %r10
    movabsq $0, %r11
    movabsq $0, %r12
    movabsq $10, %rbx
println_int_loop0:
    divq %rbx
    addq $48, %rdx
    movb %dl, (%r10)
    movabsq $0, %rdx
    addq $1, %r10
    addq $1, %r11
    cmpq %rax, %r12
    jne println_int_loop0
    movabsq $buf, %r12
    movabsq $0, %r14
    cmpq %r14, %r15
    jge println_int_eif1
    movb $45, (%r12)
    addq $1, %r12
    addq $1, %r11
println_int_eif1:
    movq %r11, strlen
println_int_loop1:
    subq $1, %r10
    movb (%r10), %r13b
    movb %r13b, (%r12)
    addq $1, %r12
    subq $1, %r11
    cmpq %r11, %r14
    jne println_int_loop1
    movabsq $buf, %rsi
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
input:
    movq stack_ptr, %r10
    subq $8, %r10
    movabsq $0, %rax
    movabsq $0, %rdi
    movabsq $buf, %rsi
    movabsq $1, %rdx
    syscall
    movabsq $0, %r15
    movabsq $0, %r11
    movb (%rsi), %r11b
    movabsq $48, %r13
    movabsq $0, %r12
    cmpb $45, %r11b
    jne input_else0
    movabsq $1, %r15
    jmp input_eif0
input_else0:
    cmpb $48, %r11b
    cmovlq %r13, %r11
    cmpb $57, %r11b
    cmovgq %r13, %r11
    subq $48, %r11
    addq %r11, %r12
input_eif0:
    movq $10, %r14
input_loop0:
    movq $0, %rax
    movq $1, %rdx
    syscall
    movabsq $0, %r11
    movb (%rsi), %r11b
    cmpb $10, %r11b
    je input_end_loop0
    movq %r12, %rax
    cqo
    imulq %r14
    movq %rax, %r12
    cmpb $48, %r11b
    cmovlq %r13, %r11
    cmpb $57, %r11b
    cmovgq %r13, %r11
    subq $48, %r11
    addq %r11, %r12
    jmp input_loop0
input_end_loop0:
    cmpb $0, %r15b
    je input_eif1
    negq %r12
input_eif1:
    movq (%r10), %r15
    movq %r12, (%r15)
    movq %r10, stack_ptr
    ret
srand:
    movq stack_ptr, %rax
    subq $8, %rax
    movq %rax, stack_ptr
    movq (%rax), %rax
    movq %rax, seed
    ret
rand:
    movq seed, %rax
    movabsq $2862933555777941757, %rbx
    movq $0, %rdx
    mulq %rbx
    movabsq $3037000493, %rbx
    addq %rbx, %rax
    movabsq $0x7fffffffffffffff, %rbx
    andq %rbx, %rax
    movq stack_ptr, %r10
    movq %rax, (%r10)
    addq $8, %r10
    movq %r10, stack_ptr
    movq %rax, seed
    ret
