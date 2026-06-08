.section .text.boot, "ax"
.global _start







_start:
    msr DAIFSet, #3
    dsb sy
    isb sy

    // Zero BSS section
    ldr x4, =__sbss
    ldr x5, =__ebss

zero_bss_loop:
    cmp x4, x5
    bhs zero_bss_done

    stp xzr, xzr, [x4], #16
    b zero_bss_loop

zero_bss_done:

    // TODO: Set stack within SRAM...
    ldr x4, =0x40400000
    mov sp, x4

    // Initialize vector table
    ldr x4, =vector_table
    msr vbar_el2, x0
    isb

    b sbl_main

halt:
    wfi
    b halt