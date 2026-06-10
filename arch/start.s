.section .text.boot, "ax"
.global _start







_start:
    mrs x4, sctlr_el2
    bic x4, x4, #(1 << 0)   // M-bit
    bic x4, x4, #(1 << 1)   // A-bit
    bic x4, x4, #(1 << 2)   // C-bit
    orr x4, x4, #(1 << 3)   // Stack alignment
    bic x4, x4, #(1 << 12)  // I-bit
    msr sctlr_el2, x4
    isb

    // Setup and switch to EL1
    mrs x4, hcr_el2
    bic x4, x4, #(1 << 0)   // Virtualization disabled
    orr x4, x4, #(1 << 3)   // Physical FIQ's are taken to EL2
    orr x4, x4, #(1 << 4)   // Physical IRQ's are taken to EL2
    orr x4, x4, #(1 << 5)   // External aborts are taken to EL2
    msr hcr_el2, x4
    isb

    mrs x4, ICC_SRE_EL2
    orr x4, x4, #(1 << 0)
    orr x4, x4, #(1 << 1)
    orr x4, x4, #(1 << 2)
    orr x4, x4, #(1 << 3)
    msr ICC_SRE_EL2, x4
    isb

    // Zero BSS section
    ldr x4, =__sbss
    ldr x5, =__ebss

zero_bss_loop:
    cmp x4, x5
    bhs zero_bss_done

    stp xzr, xzr, [x4], #16
    b zero_bss_loop

zero_bss_done:

    // TODO: Set stack within SRAM...?
    ldr x4, =0x60000000
    mov sp, x4

    // Initialize vector table
    ldr x4, =vector_table
    msr vbar_el2, x4
    isb

    msr DAIFClr, #3
    isb

    b sbl_main

halt:
    wfi
    b halt