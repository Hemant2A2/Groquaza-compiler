.global _start
_start:
    mov w0, #0        // Initialize a[0] to 0
    str w0, [sp, #0]        // Store 0 at a[0]
    mov w0, #0        // Initialize a[1] to 0
    str w0, [sp, #4]        // Store 0 at a[1]
    mov w0, #0        // Initialize a[2] to 0
    str w0, [sp, #8]        // Store 0 at a[2]
    mov w0, #0        // Initialize a[3] to 0
    str w0, [sp, #12]        // Store 0 at a[3]
    mov w0, #0        // Initialize a[4] to 0
    str w0, [sp, #16]        // Store 0 at a[4]
    mov w0, #0        // Load literal 0
    str w0, [sp, #20]        // Store i
for_start_0:
    ldr w1, [sp, #20]        // Load i
    mov w2, #5        // Load literal 5
    cmp w1, w2        // Compare operands
    b.ge for_end_1        // Exit for loop if condition false
    ldr w0, [sp, #20]        // Load value of i
    ldr w1, [sp, #20]        // Load index from i
    mov w2, #4        // Element size
    mul w1, w1, w2        // Compute index offset
    add w3, sp, #0        // Compute base address for a
    add w3, w3, w1        // Add register offset
    str w0, [w3]        // Store value into a element
    ldr w1, [sp, #20]        // Load i
    mov w2, #1        // Load literal 1
    add w0, w1, w2        // Compute addition
    str w0, [sp, #20]        // Store i
    b for_start_0        // Repeat for loop
for_end_1:
    str w0, [sp, #24]        // Store ans
    ldr w0, [sp, #24]        // Load ans
    bl print_int        // Call print_int
    mov x8, #93        // Exit syscall
    mov x0, #0        // Exit code 0
    svc #0
