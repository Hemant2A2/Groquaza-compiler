.global _start
_start:
    mov w0, #2 // Load literal 2
    str w0, [sp, #0] // Store in a
    mov w0, #3 // Load literal 3
    str w0, [sp, #4] // Store in b
    ldr w1, [sp, #0] // Load a
    ldr w2, [sp, #4] // Load b
    add w0, w1, w2 // Compute addition
    str w0, [sp, #8] // Store in c
    ldr w0, [sp, #8] // Load c
    bl print_int // Call print_int
    mov x8, #93 // Exit syscall
    mov x0, #0 // Exit code 0
    svc #0
