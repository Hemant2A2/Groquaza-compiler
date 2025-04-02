.global _start
_start:
    mov w0, #5        // Load literal 5
    str w0, [sp, #0]        // Store c
    ldr w0, [sp, #0]        // Load variable c
    bl print_int        // Print operand
    mov w0, #7        // Load literal 7
    bl print_int        // Print operand
    mov x8, #93        // Exit syscall
    mov x0, #0        // Exit code 0
    svc #0
