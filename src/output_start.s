.global _start
_start:
    mov w0, #1 // Load literal 1
    str w0, [sp, #0] // Store a
    mov w0, #2 // Load literal 2
    str w0, [sp, #4] // Store b
    ldr w1, [sp, #0] // Load a
    ldr w2, [sp, #4] // Load b
    cmp w1, w2 // Compare condition
    b.eq else_0 // Branch if condition false
    ldr w0, [sp, #0] // Load a
    bl print_int // Call print_int
    b endif_1 // Jump to end of if
else_0:
    mov w0, #0 // Load literal 0
    bl print_int // Call print_int
endif_1:
    mov x8, #93 // Exit syscall
    mov x0, #0 // Exit code 0
    svc #0
