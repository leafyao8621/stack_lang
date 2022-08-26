calc_strlen:
    ldr x9, =stack_ptr
    ldr x10, [x9]
    sub x10, x10, #8
    ldr x11, [x10]
    mov x12, #0
calc_strlen_loop0:
    ldrb w13, [x11]
    cmp x13, #0
    beq calc_strlen_end_loop0
    add x11, x11, 1
    add x12, x12, 1
    b calc_strlen_loop0
calc_strlen_end_loop0:
    ldr x9, =strlen
    str x12, [x9]
    ret
print_str:
    ldr x9, =stack_ptr
    ldr x10, [x9]
    sub x10, x10, #8
    str x10, [x9]
    ldr x11, [x10]
    mov x8, #64
    mov x0, #1
    mov x1, x11
    ldr x9, =strlen
    ldr x10, [x9]
    mov x2, x10
    svc #0
    ret
println_str:
    ldr x9, =stack_ptr
    ldr x10, [x9]
    sub x10, x10, #8
    str x10, [x9]
    ldr x11, [x10]
    mov x8, #64
    mov x0, #1
    mov x1, x11
    ldr x9, =strlen
    ldr x10, [x9]
    mov x2, x10
    svc #0
    ldr x9, =str_buf
    mov x10, #10
    strb w10, [x9]
    mov x8, #64
    mov x0, #1
    mov x1, x9
    mov x2, #1
    svc #0
    ret
print_chr:
    ldr x9, =stack_ptr
    ldr x10, [x9]
    sub x10, x10, #8
    str x10, [x9]
    ldr x11, [x10]
    ldr x12, =str_buf
    strb w11, [x12]
    mov x8, #64
    mov x0, #1
    mov x1, x12
    mov x2, #1
    svc #0
    ret
println_chr:
    ldr x9, =stack_ptr
    ldr x10, [x9]
    sub x10, x10, #8
    str x10, [x9]
    ldr x11, [x10]
    ldr x12, =str_buf
    strb w11, [x12]
    mov x8, #64
    mov x0, #1
    mov x1, x12
    mov x2, #1
    svc #0
    ldr x9, =str_buf
    mov x10, #10
    strb w10, [x9]
    mov x8, #64
    mov x0, #1
    mov x1, x9
    mov x2, #1
    svc #0
    ret
print_int:
    ldr x9, =stack_ptr
    ldr x10, [x9]
    sub x10, x10, #8
    str x10, [x9]
    ldr x11, [x10]
    mov x20, #0
    cmp x11, #0
    bge print_int_eif0
    mov x20, #1
    neg x11, x11
print_int_eif0:
    mov x12, #0
    ldr x13, =int_buf
print_int_loop0:
    mov x14, #10
    udiv x15, x11, x14
    msub x16, x15, x14, x11
    add x16, x16, #48
    strb w16, [x13]
    mov x11, x15
    add x12, x12, #1
    add x13, x13, #1
    cmp x11, #0
    bgt print_int_loop0
    mov x21, x12
    sub x13, x13, 1
    ldr x14, =str_buf
    cmp x20, #0
    beq print_int_eif1
    mov x15, #45
    strb w15, [x14]
    add x14, x14, 1
print_int_eif1:
print_int_loop1:
    ldrb w15, [x13]
    strb w15, [x14]
    sub x12, x12, #1
    sub x13, x13, #1
    add x14, x14, #1
    cmp x12, #0
    bgt print_int_loop1
    mov x8, #64
    mov x0, #1
    ldr x1, =str_buf
    mov x2, x21
    cmp x20, #0
    beq print_int_eif2
    add x2, x2, 1
print_int_eif2:
    svc #10
    ret
println_int:
    ldr x9, =stack_ptr
    ldr x10, [x9]
    sub x10, x10, #8
    str x10, [x9]
    ldr x11, [x10]
    mov x20, #0
    cmp x11, #0
    bge println_int_eif0
    mov x20, #1
    neg x11, x11
println_int_eif0:
    mov x12, #0
    ldr x13, =int_buf
println_int_loop0:
    mov x14, #10
    udiv x15, x11, x14
    msub x16, x15, x14, x11
    add x16, x16, #48
    strb w16, [x13]
    mov x11, x15
    add x12, x12, #1
    add x13, x13, #1
    cmp x11, #0
    bgt println_int_loop0
    mov x21, x12
    sub x13, x13, 1
    ldr x14, =str_buf
    cmp x20, #0
    beq println_int_eif1
    mov x15, #45
    strb w15, [x14]
    add x14, x14, 1
println_int_eif1:
println_int_loop1:
    ldrb w15, [x13]
    strb w15, [x14]
    sub x12, x12, #1
    sub x13, x13, #1
    add x14, x14, #1
    cmp x12, #0
    bgt println_int_loop1
    mov x8, #64
    mov x0, #1
    ldr x1, =str_buf
    mov x2, x21
    cmp x20, #0
    beq println_int_eif2
    add x2, x2, 1
println_int_eif2:
    svc #10
    ldr x9, =str_buf
    mov x10, #10
    strb w10, [x9]
    mov x8, #64
    mov x0, #1
    mov x1, x9
    mov x2, #1
    svc #0
    ret
input:
    ldr x9, =str_buf
    mov x8, #63
    mov x0, #0
    mov x1, x9
    mov x2, #1
    svc #0
    ldr x10, [x9]
    sub x12, x10, #48
    mov x13, #-1
    mov x14, #0
    cmp x10, #48
    csel x11, x12, x14, ge
    cmp x10, #57
    csel x11, x12, x14, le
    cmp x10, #45
    csel x11, x11, x14, ne
    cmp x10, #45
    csel x21, x11, x14, ne
    mov x20, #10
input_loop0:
    mov x8, #63
    mov x0, #0
    mov x1, x9
    mov x2, #1
    svc #0
    ldr x10, [x9]
    cmp x10, #10
    beq input_end_loop0
    mul x11, x11, x20
    sub x13, x10, #48
    mov x14, #0
    cmp x10, #48
    csel x12, x13, x14, ge
    cmp x10, #57
    csel x12, x13, x14, le
    add x11, x11, x12
    b input_loop0
input_end_loop0:
    cmp x21, #0
    bne input_eif0
    neg x11, x11
input_eif0:
    ldr x9, =stack_ptr
    ldr x10, [x9]
    sub x10, x10, #8
    ldr x12, [x10]
    str x11, [x12]
    str x10, [x9]
    ret
