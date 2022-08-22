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
