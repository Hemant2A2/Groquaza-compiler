.global _start
_start:
    mov w0, #27        // Load literal 27
    str w0, [sp, #0]        // Store a
    mov w0, #0        // Load literal 0
    str w0, [sp, #4]        // Store b
    ldr w1, [sp, #0]        // Load a
    mov w2, #30        // Load literal 30
    cmp w1, w2        // Compare operands
    b.ge else_0        // Branch if condition false
    ldr w1, [sp, #4]        // Load b
    mov w2, #1        // Load literal 1
    add w0, w1, w2        // Compute addition
    str w0, [sp, #4]        // Store b
    ldr w1, [sp, #0]        // Load a
    mov w2, #25        // Load literal 25
    cmp w1, w2        // Compare operands
    b.ge else_1        // Branch if condition false
    ldr w1, [sp, #4]        // Load b
    mov w2, #5        // Load literal 5
    add w0, w1, w2        // Compute addition
    str w0, [sp, #4]        // Store b
    b endif_2        // Jump to end of if
else_1:
endif_2:
    ldr w1, [sp, #0]        // Load a
    mov w2, #27        // Load literal 27
    cmp w1, w2        // Compare operands
    b.ne else_3        // Branch if condition false
    ldr w1, [sp, #4]        // Load b
    mov w2, #10        // Load literal 10
    add w0, w1, w2        // Compute addition
    str w0, [sp, #4]        // Store b
    b endif_4        // Jump to end of if
else_3:
endif_4:
    b endif_5        // Jump to end of if
else_0:
    ldr w1, [sp, #4]        // Load b
    mov w2, #37        // Load literal 37
    add w0, w1, w2        // Compute addition
    str w0, [sp, #4]        // Store b
endif_5:
    ldr w0, [sp, #4]        // Load b
    bl print_int        // Call print_int
    mov x8, #93        // Exit syscall
    mov x0, #0        // Exit code 0
    svc #0
