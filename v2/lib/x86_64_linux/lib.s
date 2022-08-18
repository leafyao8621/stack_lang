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
    movabsq $624, %rbx
    movq %rbx, index
    movabsq $mt, %r10
    movq (%rax), %r11
    movq %r11, (%r10)
    addq $8, %r10
    movabsq $1, %r11
    movb $30, %cl
    movabsq $1812433253, %rbx
srand_loop0:
    cmpq $624, %r11
    je srand_end_loop0
    movq -8(%r10), %r12
    movq %r12, %r13
    shrq %cl, %r13
    xorq %r13, %r12
    movq %r12, %rax
    cqo
    imulq %rbx
    addq %r11, %rax
    movq %rax, (%r10)
    addq $8, %r10
    addq $1, %r11
    jmp srand_loop0
srand_end_loop0:
    ret
twist:
    movabsq $mt, %r10
    movabsq $0, %r11
twist_loop0:
    cmpq $624, %r11
    je twist_end_loop0
    movq (%r10), %r12
    movabsq $0x80000000, %r14
    andq %r14, %r12

    cmpq $623, %r11
    jl twist_eif0
    movq 8(%r10), %r13
    jmp twist_eif0
twist_else0:
    movabsq $mt, %r14
    movq (%r14), %r13
twist_eif0:
    andq $0x7fffffff, %r13
    orq %r13, %r12

    movq %r12, %r13
    shrq $1, %r13

    movq %r12, %r14
    andq $1, %r14
    cmpq $0, %r14
    jne twist_eif1
    xorq %r12, %r13
twist_eif1:
    movabsq $mt, %r14
    movq %r11, %r15
    addq $397, %r15
    movq %r15, %rax
    cqo
    movabsq $624, %rbx
    idivq %rbx
    addq %rdx, %r14
    movq (%r14), %r14
    xorq %r14, %r13
    movq %r13, (%r10)

    addq $8, %r10
    addq $1, %r11
    jmp twist_loop0
twist_end_loop0:
    movabsq $0, %r10
    movabsq $index, %r11
    movq %r10, (%r11)
    ret
rand:
    movq index, %r10
    movabsq $624, %r11
    cmpq %r11, %r10
    jne rand_eif0
    call twist
    movabsq $mt, %r10
    movq index, %r11
    addq %r11, %r10
    movq (%r10), %r10
    // y ^= y >> u
    movq %r10, %r11
    shrq $11, %r11
    xorq %r11, %r10
    // y ^= (y << s) & b
    movq %r10, %r11
    shlq $7, %r11
    movabsq $0x9d2c5680, %r12
    andq %r12, %r11
    xorq %r11, %r10
    // y ^= (y << t) & c
    movq %r10, %r11
    shlq $15, %r11
    movabsq $0xefc60000, %r12
    andq %r12, %r11
    xorq %r11, %r10
    movq %r10, %r11
    // y ^= y >> 1
    movq %r10, %r11
    shrq $1, %r11
    xorq %r11, %r10
    movq stack_ptr, %rax
    movq %r10, (%rax)
    addq $8, %rax
    movq %rax, stack_ptr
    movq $index, %r10
    addq $1, %r10
    movq %r10, index
rand_eif0:
    ret
