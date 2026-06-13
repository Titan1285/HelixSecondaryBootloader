.section .text


.global disable_irqs
.global enable_irqs

.global icc_pmr_el1_write
.global icc_igrpen0_write
.global icc_igrpen1_write
.global icc_bpr1_write
.global icc_iar1_el1_read
.global icc_eoir1_el1_write
.global icc_ctlr_el1_write

.global cnthp_ctl_el2_read
.global cnthp_ctl_el2_write
.global cnthp_cval_el2_write 

.global ttbr0_el2_read
.global ttbr0_el2_write

.global tcr_el2_read
.global tcr_el2_write

.global sctlr_el2_read
.global sctlr_el2_write


















sctlr_el2_read:
	mrs x0, sctlr_el2
	ret

sctlr_el2_write:
	msr sctlr_el2, x0
	isb
	ret

tcr_el2_read:
	mrs x0, tcr_el2
	ret

tcr_el2_write:
	msr tcr_el2, x0
	isb
	ret

ttbr0_el2_read:
	mrs x0, ttbr0_el2
	ret

ttbr0_el2_write:
	msr ttbr0_el2, x0
	isb
	ret

disable_irqs:
	msr DAIFSet, #3
	isb
	ret

enable_irqs:
	msr DAIFClr, #3
	isb
	ret

cnthp_ctl_el2_read:
	mrs x0, cnthp_ctl_el2
	ret

cnthp_ctl_el2_write:
	msr cnthp_ctl_el2, x0
	isb
	ret

cnthp_cval_el2_write:
	msr cnthp_cval_el2, x0
	isb
	ret




icc_pmr_el1_write:
	msr ICC_PMR_EL1, x0
	isb
	ret

icc_igrpen0_write:
	msr ICC_IGRPEN0_EL1, x0
	isb
	ret

icc_igrpen1_write:
	msr ICC_IGRPEN1_EL1, x0
	isb
	ret

icc_bpr1_write:
	msr ICC_BPR1_EL1, x0
	isb
	ret

icc_iar1_el1_read:
	mrs x0, ICC_IAR1_EL1
	isb
	ret

icc_eoir1_el1_write:
	msr ICC_EOIR1_EL1, x0
	isb
	ret

icc_ctlr_el1_write:
	msr ICC_CTLR_EL1, x0
	isb
	ret





.global arch_jump_ub
arch_jump_ub:
	// x0 -> U-Boot base address

	mov x15, x0
	ldr x0, =0x40400000

	// TODO: Fix illegal instruction fetch attempting to drop to EL1...

	// mrs x4, hcr_el2
	// bic x4, x4, #(1 << 3)	// IRQ's routed to EL1
	// bic x4, x4, #(1 << 4)	// FIQ's fouted to EL1
	// bic x4, x4, #(1 << 5)	// External aborts routed to EL1 
	// msr hcr_el2, x4
	// isb

	br x15

	//mov x4, #0x39C
	//msr spsr_el2, x4
	//isb

	// Set return address to U-Boot base
	//msr elr_el2, x0
	//isb

	// Jump to U-Boot
	//eret