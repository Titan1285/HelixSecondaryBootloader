.section .text

.global icc_pmr_el3_write
.global icc_igrpen0_write
.global icc_igrpen1_write
.global icc_bpr1_write
.global icc_iar0_el3_read
.global icc_eoir0_el3_write
.global icc_ctlr_el3_write
.global disable_irqs
.global enable_irqs
.global cntfrq_el0_read

// Physical timer registers
.global cntp_ctl_el0_read
.global cntp_ctl_el0_write
.global cntpct_el0_read
.global cntp_tval_el0_read
.global cntp_tval_el0_write
.global cntp_cval_el0_read
.global cntp_cval_el0_write

// Virtual timer registers
.global cntv_ctl_el0_read
.global cntv_ctl_el0_write
.global cntvct_el0_read
.global cntv_tval_el0_read
.global cntv_tval_el0_write
.global cntv_cval_el0_read
.global cntv_cval_el0_write

.global icc_bpr0_write
.global cntps_ctl_el1_read
.global cntps_ctl_el1_write
.global cntps_tval_el1_read
.global cntps_tval_el1_write











icc_pmr_el3_write:
    msr ICC_PMR_EL1, x0
    isb
    ret

icc_igrpen0_write:
    msr ICC_IGRPEN0_EL1, x0
    isb
    ret

icc_igrpen1_write:
    msr ICC_IGRPEN1_EL3, x0
    isb
    ret

icc_bpr0_write:
	msr ICC_BPR0_EL1, x0
	isb
	ret

icc_bpr1_write:
    msr ICC_BPR1_EL1, x0
    isb
    ret

icc_iar0_el3_read:
    mrs x0, ICC_IAR0_EL1
    isb
    ret

icc_eoir0_el3_write:
    msr ICC_EOIR0_EL1, x0
    isb
    ret

icc_ctlr_el3_write:
    msr ICC_CTLR_EL3, x0
    isb
    ret

disable_irqs:
    msr DAIFSet, #3
    dsb sy
    isb sy
    ret

enable_irqs:
    msr DAIFClr, #3
    dsb sy
    isb sy
    ret



cntv_ctl_el0_read:
	mrs x0, cntv_ctl_el0
	ret

cntv_ctl_el0_write:
	msr cntv_ctl_el0, x0
	isb
	ret

cntfrq_el0_read:
	mrs x0, cntfrq_el0
	ret

cntvct_el0_read:
	mrs x0, cntvct_el0
	ret

cntv_tval_el0_read:
	mrs x0, cntv_tval_el0
	ret

cntv_tval_el0_write:
	msr cntv_tval_el0, x0
	isb
	ret

cntp_ctl_el0_read:
	mrs x0, cntp_ctl_el0
	ret

cntp_ctl_el0_write:
	msr cntp_ctl_el0, x0
	isb
	ret

cntps_ctl_el1_read:
	mrs x0, cntps_ctl_el1
	ret

cntps_ctl_el1_write:
	msr cntps_ctl_el1, x0
	isb
	ret

cntp_tval_el0_read:
	mrs x0, cntp_tval_el0
	ret

cntp_tval_el0_write:
	msr cntp_tval_el0, x0
	isb
	ret

cntps_tval_el1_read:
	mrs x0, cntps_tval_el1
	ret

cntps_tval_el1_write:
	msr cntps_tval_el1, x0
	isb
	ret

cntpct_el0_read:
	mrs x0, cntpct_el0
	ret

cntp_cval_el0_read:
	mrs x0, cntp_cval_el0
	ret

cntp_cval_el0_write:
	msr cntp_cval_el0, x0
	isb
	ret





.global arch_jump_sbl
arch_jump_sbl:
	// x0 -> sbl base address

	mrs x4, scr_el3
	orr x4, x4, #(1 << 0)	// EL2 is non-secure
	bic x4, x4, #(1 << 1)
	bic x4, x4, #(1 << 2)
	bic x4, x4, #(1 << 3)
	orr x0, x0, #(1 << 10)	// EL2 is AARCH64
	msr scr_el3, x4
	isb

	mrs x4, spsr_el3
	bic x4, x4, #(1 << 4)	// EL2 is AARCH64
	orr x4, x4, #(1 << 6)	// Mask FIQ's
	orr x4, x4, #(1 << 7)	// Mask IRQ's
	orr x4, x4, #(1 << 8)	// Mask external aborts
	orr x4, x4, #(8 << 0)	// EL2 uses SP_EL2
	msr spsr_el3, x4
	isb

	// Set return address to SBL base
	msr elr_el3, x0
	isb

	// Jump to EL2 at new address
	eret