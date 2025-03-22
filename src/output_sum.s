.global _start
_start:
    mov w0, #0 // Load literal 0
    str w0, [sp, #0] // Store sum
    mov w0, #1 // Load literal 1
    str w0, [sp, #4] // Store i
    mov w0, #10 // Load literal 10
    str w0, [sp, #8] // Store end
    mov w0, #1 // Load literal 1
    str w0, [sp, #12] // Store add
while_start_0:
    ldr w1, [sp, #4] // Load i
    ldr w2, [sp, #8] // Load end
    cmp w1, w2 // Compare condition
    b.gt while_end_1 // Exit loop if condition false
    ldr w1, [sp, #0] // Load sum
    ldr w2, [sp, #4] // Load i
    add w0, w1, w2 // Compute addition
    str w0, [sp, #0] // Store sum
    ldr w1, [sp, #4] // Load i
    ldr w2, [sp, #12] // Load add
    add w0, w1, w2 // Compute addition
    str w0, [sp, #4] // Store i
    b while_start_0 // Repeat loop
while_end_1:
    ldr w0, [sp, #0] // Load sum
    bl print_int // Call print_int
    mov x8, #93 // Exit syscall
    mov x0, #0 // Exit code 0
    svc #0
