.global _start
_start:
    mov w0, #0        // Load literal 0
    str w0, [sp, #0]        // Store n
    bl read_int        // Read integer input
    str w0, [sp, #0]        // Store input into n
    mov w0, #1        // Load literal 1
    str w0, [sp, #4]        // Store a
    ldr w1, [sp, #0]        // Load n
    ldr w2, [sp, #4]        // Load a
    add w0, w1, w2        // Compute addition
    str w0, [sp, #0]        // Store n
    ldr w0, [sp, #4]        // Load variable a
    bl print_int        // Print operand
    ldr w0, [sp, #0]        // Load variable n
    bl print_int        // Print operand
    mov x8, #93        // Exit syscall
    mov x0, #0        // Exit code 0
    svc #0
