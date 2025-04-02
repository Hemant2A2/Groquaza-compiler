.global _start
_start:
    mov w0, #2        // Load literal 2
    str w0, [sp, #0]        // Store a
    mov w0, #3        // Load literal 3
    str w0, [sp, #4]        // Store b
    ldr w1, [sp, #0]        // Load a
    ldr w2, [sp, #4]        // Load b
    add w0, w1, w2        // Compute addition
    str w0, [sp, #8]        // Store c
    ldr w1, [sp, #8]        // Load c
    mov w2, #10        // Load literal 10
    sub w0, w1, w2        // Compute subtraction
    str w0, [sp, #8]        // Store c
    ldr w0, [sp, #8]        // Load variable c
    bl print_int        // Print operand
    mov x8, #93        // Exit syscall
    mov x0, #0        // Exit code 0
    svc #0
