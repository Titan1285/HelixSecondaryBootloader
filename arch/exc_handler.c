#include <stdio.h>
#include <stdint.h>
#include <arch/arch.h>











static char *fsc_to_str(uint64_t fsc) {
    switch(fsc) {
        case 0x00:
            return "Address size fault, level 0";
            break;
        
        case 0x01:
            return "Address size fault, level 1";
            break;

        case 0x02:
            return "Address size fault, level 2";
            break;

        case 0x03:
            return "Address size fault, level 3";
            break;

        case 0x04:
            return "Translation fault, level 0";
            break;

        case 0x05:
            return "Translation fault, level 1";
            break;

        case 0x06:
            return "Translation fault, level 2";
            break;

        case 0x07:
            return "Translation fault, level 3";
            break;

        case 0x08:
            return "Access flag fault, level 0";
            break;

        case 0x09:
            return "Access flag fault, level 1";
            break;

        case 0x0A:
            return "Access flag fault, level 2";
            break;

        case 0x0B:
            return "Access flag fault, level 3";
            break;

        case 0x0C:
            return "Permission fault, level 0";
            break;

        case 0x0D:
            return "Permission fault, level 1";
            break;
        
        case 0x0E:
            return "Permission fault, level 2";
            break;

        case 0x0F:
            return "Permission fault, level 3";
            break;

        default:
            return "Undefined";
            break;
    }

    return NULL;
}

void exc_handler(cpu_ctx_t *ctx) {
    uint64_t esr = 0, ec = 0, iss = 0, fsc = 0;

    esr = ctx->esr_el1;
    ec = (esr >> 26) & 0x3F;
    iss = esr & 0x01FFFFFF;
    fsc = iss & 0x3F;


    printf("\033[031mKernel exception!\033[0m\n");
    printf("ESR: %llx FAR: %llx ELR: %llx SPSR: %llx\n\n", ctx->esr_el1, ctx->far_el1, ctx->elr_el1, ctx->spsr_el1);

    switch(ec) {
        case EC_UNKNOWN_EXCEPTION:
            printf("Unknown\n");
            break;

        case EC_WFE_WFI_TRAP:
            printf("Trapped WFI/WFE\n");
            break;

        case EC_MCR_MRC_TRAP:
        case EC_MCRR_MRRC_TRAP:
        case EC_MRRC_TRAP_ACCESS:
        case EC_MCR_MRC_TRAP_ACCESS:
            printf("Trapped MCR/MRC\n");
            break;

        case EC_LDC_STC_TRAP:
            printf("Trapped LDC/STC\n");
            break;

        case EC_SME_TRAP:
        case EC_SME_SVE_TRAP:
            printf("Trapped SME\n");
            break;

        case EC_POINTER_AUTH_TRAP:
            printf("Pointer authentication trapped\n");
            break;

        case EC_UNKNOWN_INSTRUCTION_TRAP:
            printf("Unknown instruction/Illegal execution trapped\n");
            break;

        case EC_BRANCH_TARGET_EXCEPTION:
            printf("Branch target trapped\n");
            break;

        case EC_MSR_MRS_TRAP:
            printf("Trapped MSR/MRS\n");
            break;

        case EC_SVE_TRAP:
            printf("Trapped SVE\n");
            break;

        case EC_ERET_TRAP:
            printf("Trapped ERET\n");
            break;

        case EC_PAC_FAIL:
            printf("PAC Fail\n");
            break;

        case EC_INSTRUCTION_ABORT_LOW_EL:
            printf("Instruction abort in lower exception level\n");
            break;

        case EC_INSTRUCTION_ABORT_SAME_EL:
            printf("Instruction abort in without change in exception level\n");
            break;
        
        case EC_PC_ALIGNMENT_FAULT:
            printf("PC alignment fault\n");
            break;

        case EC_DATA_ABORT_LOW_EL:
            printf("Data abort from lower exception level, reason: %s\n", fsc_to_str(fsc));
            break;

        case EC_DATA_ABORT_SAME_EL:
            printf("Data abort without a change in exception level, reason: %s\n", fsc_to_str(fsc));
            break;

        case EC_SERROR:
            printf("SError\n");
            break;

        case EC_BREAKPOINT_LOW_EL:
            printf("Breakpoint from lower exception level\n");
            break;

        case EC_BREAKPOINT_SAME_EL:
            printf("Breakpoint without a change in exception level\n");
            break;

        case EC_TRAP_FPU_ARM64:
            // TODO: Enable FPU registers if this occurs...
            printf("Trapped FPU\n");
            break;

        case EC_BRK_ARM64:
            printf("BRK instruction\n");
            break;

        default:
            printf("???\n");
            break;
    }

    while(1);
}