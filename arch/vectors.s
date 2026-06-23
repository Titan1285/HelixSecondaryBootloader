.section .text.vectors, "ax"
.global vector_table







.macro __table_entry label
    .align 7
    b \label
.endm

.macro __unimplemented_entry
    .align 7
    wfe
    b .
.endm

.macro __save_context
    stp x0, x1, [sp, #-16]!
    stp x2, x3, [sp, #-16]!
    stp x4, x5, [sp, #-16]!
    stp x6, x7, [sp, #-16]!
    stp x8, x9, [sp, #-16]!
    stp x10, x11, [sp, #-16]!
    stp x12, x13, [sp, #-16]!
    stp x14, x15, [sp, #-16]!
    stp x16, x17, [sp, #-16]!
    stp x18, x19, [sp, #-16]!
    stp x20, x21, [sp, #-16]!
    stp x22, x23, [sp, #-16]!
    stp x24, x25, [sp, #-16]!
    stp x26, x27, [sp, #-16]!
    stp x28, x29, [sp, #-16]!
    stp x30, xzr, [sp, #-16]!
    
    mrs x0, esr_el2
    mrs x1, far_el2
    stp x0, x1, [sp, #-16]!

    mrs x0, elr_el2
    mrs x1, spsr_el2
    stp x0, x1, [sp, #-16]!
.endm

.macro __restore_context
    ldp x0, x1, [sp], #16
    msr elr_el2, x0
    msr spsr_el2, x1

    // Drop ESR and FAR
    ldp x0, x1, [sp], #16

    ldp x30, xzr, [sp], #16
    ldp x28, x29, [sp], #16
    ldp x26, x27, [sp], #16
    ldp x24, x25, [sp], #16
    ldp x22, x23, [sp], #16
    ldp x20, x21, [sp], #16
    ldp x18, x19, [sp], #16
    ldp x16, x17, [sp], #16
    ldp x14, x15, [sp], #16
    ldp x12, x13, [sp], #16
    ldp x10, x11, [sp], #16
    ldp x8, x9, [sp], #16
    ldp x6, x7, [sp], #16
    ldp x4, x5, [sp], #16
    ldp x2, x3, [sp], #16
    ldp x0, x1, [sp], #16
.endm










.align 11
vector_table:
    // ELx SP_EL0
    __table_entry sync_sp_el0
    __table_entry irq_sp_el0
    __table_entry fiq_sp_el0
    __table_entry serror_sp_el0

    // Elx SP_EL1
    __table_entry sync_elx
    __table_entry irq_elx
    __table_entry fiq_elx
    __table_entry serror_elx

    // EL0 ARM64
    __table_entry sync_el0
    __table_entry irq_el0
    __table_entry fiq_el0
    __table_entry serror_el0

    // EL0 ARM32
    __unimplemented_entry
    __unimplemented_entry
    __unimplemented_entry
    __unimplemented_entry



sync_sp_el0:
    b .

irq_sp_el0:
    b .

fiq_sp_el0:
    b .

serror_sp_el0:
    b .



sync_elx:
    __save_context

    mov x0, sp
    bl exc_handler

    __restore_context
    eret

irq_elx:
    msr DAIFSet, #2
    isb

    // Save current context
    __save_context

    bl irq_handler

    __restore_context
    
    msr DAIFClr, #2
    isb

    eret

fiq_elx:
    b .

serror_elx:
    b .



sync_el0:
    b .

irq_el0:
    b .

fiq_el0:
    b .

serror_el0:
    b .