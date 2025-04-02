.global _start
_start:
    mov w0, #1        // Load literal 1
    str w0, [sp, #0]        // Store a
    mov w0, #7        // Load literal 7
    str w0, [sp, #4]        // Store b
    ldr w1, [sp, #4]        // Load b
    ldr w2, [sp, #0]        // Load a
    sub w0, w1, w2        // Compute subtraction
    str w0, [sp, #8]        // Store c
    ldr w0, [sp, #8]        // Load variable c
    bl print_int        // Print operand
    mov x8, #93        // Exit syscall
    mov x0, #0        // Exit code 0
    svc #0
