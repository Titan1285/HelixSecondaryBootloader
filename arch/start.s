.section .text.boot, "ax"
.global _start







_start:
    // Setup and switch to EL1
    mrs x4, hcr_el2
    bic x4, x4, #(1 << 0)   // Virtualization disabled
    bic x4, x4, #(1 << 1)
    bic x4, x4, #(1 << 3)   // Physical IRQ's are not taken to EL2, virtual FIQ's are disabled
    bic x4, x4, #(1 << 4)   // Physical IRQ's are not taken to EL2/EL3, virtual FIQ's are disabled
    bic x4, x4, #(1 << 5)   // External aborts are not taken to EL2
    bic x4, x4, #(1 << 12)  // Default cachability
    msr hcr_el2, x4
    isb

    mrs x4, spsr_el2
    bic x4, x4, #(1 << 4)   // EL1 is AARCH64
    orr x4, x4, #(1 << 6)   // FIQ's masked in EL1
    orr x4, x4, #(1 << 7)   // IRQ's masked in EL1
    orr x4, x4, #(1 << 8)   // External aborts masked in EL1
    orr x4, x4, #(8 << 0)   // EL1 uses SP_EL1
    msr spsr_el2, x4
    isb

    // Set return address
    adr x4, in_el1
    msr elr_el2, x4
    isb

    // Drop to EL1
    eret

in_el1:
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

    // TODO: Set stack within SRAM...?
    ldr x4, =0x60000000
    mov sp, x4

    // Initialize vector table
    ldr x4, =vector_table
    msr vbar_el2, x0
    isb

    b sbl_main

halt:
    wfi
    b halt