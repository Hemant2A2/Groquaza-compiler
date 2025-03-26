.global _start
_start:
    mov w0, #42        // Load literal 42
    str w0, [sp, #0]        // Store a
    mov w0, #79        // Load literal 79
    str w0, [sp, #4]        // Store b
    mov w0, #47        // Load literal 47
    str w0, [sp, #8]        // Store d
while_start_0:
    ldr w1, [sp, #0]        // Load a
    mov w2, #50        // Load literal 50
    cmp w1, w2        // Compare operands
    b.ge while_end_1        // Exit loop if condition false
    ldr w1, [sp, #0]        // Load a
    mov w2, #1        // Load literal 1
    add w0, w1, w2        // Compute addition
    str w0, [sp, #0]        // Store a
    ldr w1, [sp, #0]        // Load a
    ldr w2, [sp, #8]        // Load d
    cmp w1, w2        // Compare condition
    b.ne else_2        // Branch if condition false
    ldr w1, [sp, #4]        // Load b
    mov w2, #1        // Load literal 1
    add w0, w1, w2        // Compute addition
    str w0, [sp, #4]        // Store b
    b endif_3        // Jump to end of if
else_2:
endif_3:
    b while_start_0        // Repeat loop
while_end_1:
    ldr w1, [sp, #0]        // Load a
    ldr w2, [sp, #4]        // Load b
    add w0, w1, w2        // Compute addition
    str w0, [sp, #12]        // Store c
    ldr w0, [sp, #12]        // Load c
    bl print_int        // Call print_int
    mov x8, #93        // Exit syscall
    mov x0, #0        // Exit code 0
    svc #0
