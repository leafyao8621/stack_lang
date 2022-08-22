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
