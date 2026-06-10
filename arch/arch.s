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