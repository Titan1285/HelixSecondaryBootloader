.section .text
.global flush_cache_range
.global get_icache_line_size
.global get_dcache_line_size









get_icache_line_size:
    mrs x1, ctr_el0
    ubfx x1, x1, #0, #4 // Extract IminLine bits
    mov x0, #4
    lsl x0, x0, x1      // x0 = 4 << IminLine
    ret

get_dcache_line_size:
    mrs x1, ctr_el0
    ubfx x1, x1, #16, #4 // Extract DminLine
    mov x0, #4
    lsl x0, x0, x1      // x0 = 4 << DminLine
    ret

flush_cache_range:
    // x0 = address, x1 = length

    // Save original address in x2
    mov     x2, x0

    // Read CTR_EL0
    mrs     x3, ctr_el0

    // Extract DminLine
    ubfx    x4, x3, #16, #4        // DminLine
    mov     x5, #4
    lsl     x5, x5, x4             // x5 = D-cache line size

    // Extract IminLine
    ubfx    x6, x3, #0, #4         // IminLine
    mov     x7, #4
    lsl     x7, x7, x6             // x7 = I-cache line size

    // Align start address down
    sub     x8, x2, #1
    bic     x2, x8, x5             // align to D-cache line

    // Compute end address
    add     x9, x2, x1

1:  // D-cache clean loop
    dc      cvau, x2
    add     x2, x2, x5
    cmp     x2, x9
    blt    1b

    dsb     ish

    // Reset x2 to start
    mov     x2, x0
    sub     x8, x2, #1
    bic     x2, x8, x7             // align to I line

    // Compute end again
    add     x9, x2, x1

2:  // I-cache invalidate loop
    ic      ivau, x2
    add     x2, x2, x7
    cmp     x2, x9
    blt    2b

    dsb     ish
    isb
    ret