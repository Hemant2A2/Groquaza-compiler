.global _start
_start:
    mov w0, #0        // Load literal 0
    str w0, [sp, #0]        // Store sum
    mov w0, #2        // Load literal 2
    str w0, [sp, #4]        // Store i
for_start_0:
    ldr w1, [sp, #4]        // Load i
    mov w2, #10        // Load literal 10
    cmp w1, w2        // Compare operands
    b.gt for_end_1        // Exit for loop if condition false
    ldr w1, [sp, #0]        // Load sum
    ldr w2, [sp, #4]        // Load i
    add w0, w1, w2        // Compute addition
    str w0, [sp, #0]        // Store sum
    ldr w1, [sp, #4]        // Load i
    mov w2, #2        // Load literal 2
    add w0, w1, w2        // Compute addition
    str w0, [sp, #4]        // Store i
    b for_start_0        // Repeat for loop
for_end_1:
    ldr w0, [sp, #0]        // Load sum
    bl print_int        // Call print_int
    mov x8, #93        // Exit syscall
    mov x0, #0        // Exit code 0
    svc #0
