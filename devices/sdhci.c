#include <devices/sdhci.h>
#include <devices/gicv3.h>
#include <arch/irq.h>
#include <stdio.h>
#include <mm/alloc.h>
#include <stdlib.h>










static sdhci_t *sdhci = NULL;







static inline uint64_t sdhci_cdns_read64(uint64_t offset) {
    return *(volatile uint32_t *)((uintptr_t)HELIX_SDHCI_BASE + offset);
}

static inline void sdhci_cdns_write64(uint64_t offset, uint64_t val) {
    *(volatile uint64_t *)((uintptr_t)HELIX_SDHCI_BASE + offset) = val;
}

static inline uint32_t sdhci_cdns_read32(uint32_t offset) {
    return *(volatile uint32_t *)((uintptr_t)HELIX_SDHCI_BASE + offset);
}

static inline void sdhci_cdns_write32(uint32_t offset, uint32_t val) {
    *(volatile uint32_t *)((uintptr_t)HELIX_SDHCI_BASE + offset) = val;
}

static inline uint16_t sdhci_cdns_read16(uint16_t offset) {
    return *(volatile uint32_t *)((uintptr_t)HELIX_SDHCI_BASE + offset);
}

static inline void sdhci_cdns_write16(uint16_t offset, uint16_t val) {
    *(volatile uint16_t *)((uintptr_t)HELIX_SDHCI_BASE + offset) = val;
}

static inline uint8_t sdhci_cdns_read8(uint8_t offset) {
    return *(volatile uint32_t *)((uintptr_t)HELIX_SDHCI_BASE + offset);
}

static inline void sdhci_cdns_write8(uint8_t offset, uint8_t val) {
    *(volatile uint8_t *)((uintptr_t)HELIX_SDHCI_BASE + offset) = val;
}



static inline uint64_t sdhci_read64(uint64_t offset) {
    return *(volatile uint64_t *)((uintptr_t)HELIX_SDHCI_BASE + SDHCI_CADENCE_CORE_ADDR + offset);
}

static inline void sdhci_write64(uint64_t offset, uint64_t val) {
    *(volatile uint64_t *)((uintptr_t)HELIX_SDHCI_BASE + SDHCI_CADENCE_CORE_ADDR + offset) = val;
}

static inline uint32_t sdhci_read32(uint32_t offset) {
    return *(volatile uint32_t *)((uintptr_t)HELIX_SDHCI_BASE + SDHCI_CADENCE_CORE_ADDR + offset);
}

static inline void sdhci_write32(uint32_t offset, uint32_t val) {
    *(volatile uint32_t *)((uintptr_t)HELIX_SDHCI_BASE + SDHCI_CADENCE_CORE_ADDR + offset) = val;
}

static inline uint16_t sdhci_read16(uint16_t offset) {
    return *(volatile uint16_t *)((uintptr_t)HELIX_SDHCI_BASE + SDHCI_CADENCE_CORE_ADDR + offset);
}

static inline void sdhci_write16(uint16_t offset, uint16_t val) {
    *(volatile uint16_t *)((uintptr_t)HELIX_SDHCI_BASE + SDHCI_CADENCE_CORE_ADDR + offset) = val;
}

static inline uint8_t sdhci_read8(uint8_t offset) {
    return *(volatile uint8_t *)((uintptr_t)HELIX_SDHCI_BASE + SDHCI_CADENCE_CORE_ADDR + offset);
}

static inline void sdhci_write8(uint8_t offset, uint8_t val) {
    *(volatile uint8_t *)((uintptr_t)HELIX_SDHCI_BASE + SDHCI_CADENCE_CORE_ADDR + offset) = val;
}










void sdhci_reset(void) {
    int timeout = 100000;

    // Trigger SDHCI controller reset
    sdhci_write8(SDHCI_SOFTWARE_RESET, SDHCI_RESET_ALL);

    // Wait for hardware to clear reset bit
    while(sdhci_read8(SDHCI_SOFTWARE_RESET) & SDHCI_RESET_ALL) {
        timeout--;

        if (timeout == 0) {
            printf("SDHCI reset failed!\n");
            break;
        }
    }

    // NOTE:
    // Now that this was changed to SDHCI cadence, we have to switch the device back into generic SD operations
    // This was done, so that I can still use U-Boot for the MMC, but won't have to re-write my SDHCI code, or have to
    // implement a new SDHCI driver in U-Boot

    // Reset EMMC
    uint32_t hrs00 = sdhci_cdns_read32(SDHCI_CDNS_HRS00);
    sdhci_cdns_write32(SDHCI_CDNS_HRS00,  hrs00 | SDHCI_CDNS_HRS00_SWR);

    // Wait for reset bit to clear
    while(sdhci_cdns_read32(SDHCI_CDNS_HRS00) & SDHCI_CDNS_HRS00_SWR);

    // Set to generic SD operation
    uint32_t hrs06 = sdhci_cdns_read32(SDHCI_CDNS_HRS06);
    hrs06 &= ~(SDHCI_CDNS_HRS06_MODE_MASK);
    hrs06 |= SDHCI_CDNS_HRS06_MODE_SD;
    sdhci_cdns_write32(SDHCI_CDNS_HRS06, hrs06);
}

void sdhci_set_clock(uint32_t target) {
    uint32_t divisor = 1;
    uint32_t clk_ctrl = 0;
    int timeout = 10000;




    if (target == 0) {
        return;
    }

    divisor = sdhci->base_clock / target;

    if (sdhci->base_clock % target) {
        divisor++;
    }

    divisor = (divisor + 1) / 2;

    if (divisor > 0x3FF) {
        divisor = 0x3FF;
    }

    // Disable internal clock
    clk_ctrl = sdhci_read16(SDHCI_CLOCK_CONTROL);
    clk_ctrl &= ~(SDHCI_INTERNAL_CLOCK_EN);
    sdhci_write16(SDHCI_CLOCK_CONTROL, clk_ctrl);

    // Setup new divisor
    clk_ctrl &= ~(0xFF00);
    clk_ctrl |= ((divisor & 0xFF) << 8);
    sdhci_write16(SDHCI_CLOCK_CONTROL, clk_ctrl);

    // Enable internal clock
    clk_ctrl |= SDHCI_INTERNAL_CLOCK_EN;
    sdhci_write16(SDHCI_CLOCK_CONTROL, clk_ctrl);
    
    // Wait for clock to become stable
    while(!(sdhci_read16(SDHCI_CLOCK_CONTROL) & SDHCI_INTERNAL_CLOCK_STABLE)) {
        timeout--;

        if (timeout == 0) {
            printf("SDHCI clock never stablized!\n");
            return;
        }
    }

    // Enable SD clock
    clk_ctrl |= SDHCI_SD_CLOCK_EN;
    sdhci_write16(SDHCI_CLOCK_CONTROL, clk_ctrl);
}

static void sdhci_wait_cmd_ready(void) {
    while(sdhci_read32(SDHCI_PRESENT) & SDHCI_CMD_INHIBIT);
}

static void sdhci_wait_power_stable(void) {
    uint32_t timeout = 100000;


    while(timeout--) {
        if (sdhci_read32(SDHCI_PRESENT) & SDHCI_CARD_INSERTED) {
            // Card state is now stable
            return;
        }

        if (timeout == 0) {
            printf("SDHCI power wait timeout exceeded!\n");
            return;
        }
    }
}

static int sdhci_send_cmd(uint8_t cmd_idx, uint32_t args, uint16_t flags) {
    int timeout = 100000;


    sdhci_wait_cmd_ready();

    sdhci_write32(SDHCI_ARGUMENT, args);

    uint16_t cmd = (cmd_idx << 8) | flags;
    sdhci_write16(SDHCI_COMMAND, cmd);

    while(!(sdhci_read16(SDHCI_INTERRUPT_STATUS) & SDHCI_CMD_COMPLETED)) {
        timeout--;

        if (timeout == 0) {
            printf("SDHCI CMD timeout!\n");
            return -1;
        }
    }

    sdhci_write16(SDHCI_INTERRUPT_STATUS, SDHCI_CMD_COMPLETED); 

    sdhci_write8(SDHCI_SOFTWARE_RESET, SDHCI_RESET_CMD | SDHCI_RESET_DAT);

    return 0;
}

void sdhci_init(void) {
    uint8_t power_ctrl = 0;


    sdhci = alloc(sizeof(sdhci_t));

    if (!sdhci) {
        printf("Failed to allocate SDHCI struct!\n");
        return;
    }

    sdhci->base_clock = SDHCI_BASE_CLOCK;
    sdhci->block_size = 0x0200;

    // Rest SDHCI controller
    sdhci_reset();

    // Initialize clock in low-speed (after card is stable, do high-speed)
    sdhci_set_clock(SDHCI_LOW_SPEED_MHZ);
    
    // Select card voltage (3.3V)
    power_ctrl = sdhci_read8(SDHCI_POWER_CONTROL);
    power_ctrl |= SDHCI_SDBUS_VOLTAGE_VDD1_3_3;
    sdhci_write8(SDHCI_POWER_CONTROL, power_ctrl);
    
    // Enable power
    power_ctrl |= SDHCI_SDBUS_POWER_VDD1;
    sdhci_write8(SDHCI_POWER_CONTROL, power_ctrl);

    // Wait for power to stablize
    sdhci_wait_power_stable();

    uint16_t normal_ints = SDHCI_CARD_INTERRUPT_SIGNAL_EN |
        SDHCI_BUFFER_READ_SIGNAL_EN |
        SDHCI_BUFFER_WRITE_SIGNAL_EN |
        SDHCI_TRANFER_COMPLETE_SINGAL_EN |
        SDHCI_CMD_COMPLETE_SIGNAL_EN;

    // Clear any pending interrupts
    sdhci_write16(SDHCI_INTERRUPT_STATUS, 0xFFFF);
    
    // Enable interupts
    sdhci_write16(SDHCI_NORMAL_INTERRUPT_STATUS_EN, normal_ints);
    sdhci_write16(SDHCI_NORMAL_INTERRUPT_SIGNAL_EN, normal_ints);

    int ret = sdhci_send_cmd(0, 0, 0x0000);

    if (ret < 0) {
        printf("SDHCI CMD0 failed!\n");
        return;
    }

    sdhci_send_cmd(8, 0x1AA, 0x0001);

    uint32_t resp, timeout = 500000;

    while(timeout--) {
        while(sdhci_read32(SDHCI_PRESENT) & SDHCI_CMD_INHIBIT);

        ret = sdhci_send_cmd(55, 0, 0x0001);

        if (ret < 0) {
            printf("CMD55 failed!\n");
            return;
        }

        while(sdhci_read32(SDHCI_PRESENT) & SDHCI_CMD_INHIBIT);

        ret = sdhci_send_cmd(41, 0x40300000, 0x0003);

        if (ret < 0) {
            printf("CMD41 failed!\n");
            return;
        }

        resp = sdhci_read32(SDHCI_RESPONSE);

        if (resp & (1UL << 31UL)) {
            printf("ACMD41 complete! (resp = %x)\n", resp);
            break;
        } else {
            printf("ACMD41 failed! (resp = %x)\n", resp);
            return;
        }

        if (timeout == 0) {
            printf("ACMD41 timeout!\n");
            return;
        }
    }

    ret = sdhci_send_cmd(2, 0, 0x000E);

    if (ret < 0) {
        printf("CMD2 failed!\n");
        return;
    }

    uint32_t resp0 = sdhci_read32(SDHCI_RESPONSE0);
    uint32_t resp1 = sdhci_read32(SDHCI_RESPONSE1);
    uint32_t resp2 = sdhci_read32(SDHCI_RESPONSE2);
    uint32_t resp3 = sdhci_read32(SDHCI_RESPONSE3);

    sdhci->cid[0]  = (resp3 >> 24) & 0xFF; // MID
    sdhci->cid[1]  = (resp3 >> 16) & 0xFF; // OID [1]
    sdhci->cid[2]  = (resp3 >> 8)  & 0xFF; // OID [0]
    sdhci->cid[3]  = (resp3)       & 0xFF; // PNM [4]
    sdhci->cid[4]  = (resp2 >> 24) & 0xFF; // PNM [3]
    sdhci->cid[5]  = (resp2 >> 16) & 0xFF; // PNM [2]
    sdhci->cid[6]  = (resp2 >> 8)  & 0xFF; // PNM [1]
    sdhci->cid[7]  = (resp2)       & 0xFF; // PNM [0]
    sdhci->cid[8]  = (resp1 >> 24) & 0xFF; // PRV
    sdhci->cid[9]  = (resp1 >> 16) & 0xFF; // PSN [3]
    sdhci->cid[10] = (resp1 >> 8)  & 0xFF; // PSN [2]
    sdhci->cid[11] = (resp1)       & 0xFF; // PSN [1]
    sdhci->cid[12] = (resp0 >> 24) & 0xFF; // PSN [0]
    sdhci->cid[13] = (resp0 >> 16) & 0xFF; // MDT [1] (Year)
    sdhci->cid[14] = (resp0 >> 8)  & 0xFF; // MDT [0] (Month)
    sdhci->cid[15] = (resp0)       & 0xFF; // CRC7 + End bitcid[0]  = (resp3 >> 24) & 0xFF; // MID
    sdhci->cid[1]  = (resp3 >> 16) & 0xFF; // OID [1]
    sdhci->cid[2]  = (resp3 >> 8)  & 0xFF; // OID [0]
    sdhci->cid[3]  = (resp3)       & 0xFF; // PNM [4]
    sdhci->cid[4]  = (resp2 >> 24) & 0xFF; // PNM [3]
    sdhci->cid[5]  = (resp2 >> 16) & 0xFF; // PNM [2]
    sdhci->cid[6]  = (resp2 >> 8)  & 0xFF; // PNM [1]
    sdhci->cid[7]  = (resp2)       & 0xFF; // PNM [0]
    sdhci->cid[8]  = (resp1 >> 24) & 0xFF; // PRV
    sdhci->cid[9]  = (resp1 >> 16) & 0xFF; // PSN [3]
    sdhci->cid[10] = (resp1 >> 8)  & 0xFF; // PSN [2]
    sdhci->cid[11] = (resp1)       & 0xFF; // PSN [1]
    sdhci->cid[12] = (resp0 >> 24) & 0xFF; // PSN [0]
    sdhci->cid[13] = (resp0 >> 16) & 0xFF; // MDT [1] (Year)
    sdhci->cid[14] = (resp0 >> 8)  & 0xFF; // MDT [0] (Month)
    sdhci->cid[15] = (resp0)       & 0xFF; // CRC7 + End bit

    char pnm[6];
    pnm[0] = sdhci->cid[4];
    pnm[1] = sdhci->cid[5];
    pnm[2] = sdhci->cid[6];
    pnm[3] = sdhci->cid[7];
    pnm[4] = ' ';
    pnm[5] = '\0'; // Null terminator

    printf("SDHCI: Product name: %s\n", pnm);

    uint8_t major = (sdhci->cid[8] >> 4) & 0x0F;
    uint8_t minor = sdhci->cid[8] & 0x0F;

    printf("SDHCI revision: %d.%d\n", major, minor);

    ret = sdhci_send_cmd(3, 0, 0x0001);

    if (ret < 0) {
        printf("CMD3 failed!\n");
    }

    resp = sdhci_read32(SDHCI_RESPONSE);
    sdhci->rca = (resp >> 16) & 0xFFFF;

    uint32_t arg = ((uint32_t)sdhci->rca) << 16;

    ret = sdhci_send_cmd(9, arg, SDHCI_RESP_TYPE_LEN_136 | SDHCI_CMD_CRC_CHK_EN);

    if (ret < 0) {
        printf("CMD9 failed!\n");
        return;
    }

    sdhci->csd[0] = sdhci_read32(SDHCI_RESPONSE0);
    sdhci->csd[1] = sdhci_read32(SDHCI_RESPONSE1);
    sdhci->csd[2] = sdhci_read32(SDHCI_RESPONSE2);
    sdhci->csd[3] = sdhci_read32(SDHCI_RESPONSE3);

    ret = sdhci_send_cmd(7, arg, SDHCI_RESP_TYPE_LEN_48 | SDHCI_CMD_CRC_CHK_EN | SDHCI_CMD_INDEX_CHK_EN);

    if (ret < 0) {
        printf("CMD7 failed!\n");
        return;
    }

    // Disable clock
    uint16_t clk = sdhci_read16(SDHCI_CLOCK_CONTROL);
    clk &= ~(SDHCI_SD_CLOCK_EN);
    sdhci_write16(SDHCI_CLOCK_CONTROL, clk);

    // Re-enable clock at 25MHz
    sdhci_set_clock(SDHCI_HIGH_SPEED_MHZ);

    // Enable 4-bit mode
    uint8_t host_ctrl = sdhci_read8(SDHCI_HOST_CONTROL);
    host_ctrl |= SDHCI_DATA_TRANFER_WIDTH_4BIT;
    sdhci_write8(SDHCI_HOST_CONTROL, host_ctrl);

    uint32_t rca_arg = ((uint32_t)sdhci->rca << 16);
    ret = sdhci_send_cmd(55, rca_arg, SDHCI_RESP_TYPE_LEN_48 | SDHCI_CMD_CRC_CHK_EN);

    if (ret < 0) {
        printf("SDHCI CMD55 failed!\n");
        return;
    }

    ret = sdhci_send_cmd(6, 0x2, SDHCI_RESP_TYPE_LEN_48 | SDHCI_CMD_CRC_CHK_EN);

    if (ret < 0) {
        printf("SDHCI CMD6 failed!\n");
        return;
    }

    irq_register(SDHCI_IRQ, sdhci_irq_handler);
    gic_enable_irq(SDHCI_IRQ);
    gic_set_priority(SDHCI_IRQ, PRIORITY_DEFAULT);
}

int sdhci_read_block(uint32_t lba, uint8_t *buf) {
    int ret = 0;
    uint16_t mode = SDHCI_BLOCK_EN | SDHCI_TRANSFER_DIRECT_SEL;






    // Setup block size and number of blocks to read
    sdhci_write16(SDHCI_BLOCK_SIZE, sdhci->block_size);
    sdhci_write16(SDHCI_BLOCK_COUNT, 1);

    // Specify transfer mode
    sdhci_write16(SDHCI_TRANSFER_MODE, mode);

    // NOTE: Argument is the LBA address of the EMMC/SD
    ret = sdhci_send_cmd(17, lba * 512, SDHCI_RESP_TYPE_LEN_48 | SDHCI_CMD_CRC_CHK_EN | SDHCI_DATA_PRESENT_SEL);

    if (ret < 0) {
        return ret;
    }

    // Wait for data to become available
    while(!(sdhci_read16(SDHCI_INTERRUPT_STATUS) & SDHCI_BUFFER_READ_READY));

    // Read data from buffer
    uint32_t *data = (uint32_t *)buf;
    for (uint32_t i = 0; i < (512 / 4); i++) {
        *data++ = sdhci_read32(SDHCI_BUFFER);
    }

    // Ack buffer read
    sdhci_write16(SDHCI_INTERRUPT_STATUS, SDHCI_BUFFER_READ_READY | SDHCI_TRANSFER_COMPLETE);

    return 0;
}

int sdhci_read_blocks(uint32_t lba, uint32_t length, uint8_t *buf) {
    int ret = 0;

    // TODO: Eventually replace this with CMD18. It works fine for now, but I'm done fucking with CMD18

    uint32_t blocks = length / sdhci->block_size;
    uint32_t lba_base = lba;

    uint32_t i = 0;
    while(blocks > 0) {
        ret = sdhci_read_block(lba_base, buf + (i * sdhci->block_size));

        if (ret < 0) {
            printf("Failed to read LBA %x\n", lba_base);
            return ret;
        }

        i++;
        lba_base++;
        blocks--;
    }

    return 0;
}

int sdhci_write_block(uint32_t lba, uint8_t *buf) {
    int ret = 0;
    uint16_t mode = SDHCI_BLOCK_EN | SDHCI_TRANSFER_DIRECT_SEL;






    // Setup block size and number of blocks to read
    sdhci_write16(SDHCI_BLOCK_SIZE, sdhci->block_size);
    sdhci_write16(SDHCI_BLOCK_COUNT, 1);

    // Specify transfer mode
    sdhci_write16(SDHCI_TRANSFER_MODE, mode);

    // NOTE: Argument is the LBA address of the EMMC/SD
    ret = sdhci_send_cmd(24, lba * 512, SDHCI_RESP_TYPE_LEN_48 | SDHCI_CMD_CRC_CHK_EN | SDHCI_DATA_PRESENT_SEL);

    if (ret < 0) {
        return ret;
    }

    // Wait for data to become available
    while(!(sdhci_read16(SDHCI_INTERRUPT_STATUS) & SDHCI_BUFFER_WRITE_READY));

    // Read data from buffer
    uint32_t *data = (uint32_t *)buf;
    for (uint32_t i = 0; i < (512 / 4); i++) {
        sdhci_write32(SDHCI_BUFFER, *data++);
    }

    // Ack buffer write
    sdhci_write16(SDHCI_INTERRUPT_STATUS, SDHCI_BUFFER_WRITE_READY | SDHCI_TRANSFER_COMPLETE);

    return 0;
}

int sdhci_write_blocks(uint32_t lba, uint32_t length, uint8_t *buf) {
    int ret = 0;

    uint32_t blocks = length / sdhci->block_size;
    uint32_t lba_base = lba;

    uint32_t i = 0;
    while(blocks > 0) {
        ret = sdhci_write_block(lba_base, buf + (i * sdhci->block_size));

        if (ret < 0) {
            printf("Failed to write LBA %x\n", lba_base);
            return ret;
        }

        i++;
        lba_base++;
        blocks--;
    }

    return 0;
}

void sdhci_irq_handler(void) {
    printf("SDHCI IRQ recieved!\n");
    while(1);
} 