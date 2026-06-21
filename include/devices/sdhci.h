#ifndef SDHCI_H
#define SDHCI_H

#include <stdint.h>





#define SDHCI_IRQ 61
#define HELIX_SDHCI_BASE 0x01031000

#define SDHCI_CADENCE_CORE_ADDR     0x200


#define SDHCI_DMA_ADDR              0x000           // 64-bytes

#define SDHCI_BLOCK_SIZE            0x004           // 16-bytes
#define SDHCI_BLOCK_COUNT           0x006           // 16-bytes

#define  SDHCI_MAKE_BLKSZ(dma, blksz)   (((dma & 0x7) << 12) | (blksz & 0xFFF))
#define  SDHCI_DEFAULT_BOUNDARY_SIZE	(512 * 1024)
#define  SDHCI_DEFAULT_BOUNDARY_ARG     7

#define SDHCI_TRANSFER_MODE         0x00C           // 16-bytes
#define SDHCI_RESP_INT_DISABLE      (1UL << 8UL)
#define SDHCI_RESP_ERR_CHK_EN       (1UL << 7UL)
#define SDHCI_RESP_TYPE             (1UL << 6UL)
#define SDHCI_MULTI_BLOCK_SEL       (1UL << 5UL)
#define SDHCI_TRANFER_DIRECT_SEL    (1UL << 4UL)
#define SDHCI_AUTO_CMD12_EN         (1UL << 3UL)
#define SDHCI_AUTO_CMD23_EN         (2UL << 3UL)
#define SHDIC_AUTO_CMD_SEL          (3UL << 3UL)
#define SDHCI_BLOCK_EN              (1UL << 1UL)
#define SDHCI_DMA_EN                (1UL << 0UL)

#define SDHCI_COMMAND               0x00E           // 16-bytes
#define SDHCI_CMD_TYPE_SUSPEND      (1UL << 6UL)
#define SDHCI_CMD_TYPE_RESUME       (2UL << 6UL)
#define SDHCI_CMD_TYPE_ABORT        (3UL << 6UL)
#define SDHCI_DATA_PRESENT_SEL      (1UL << 5UL)
#define SDHCI_CMD_INDEX_CHK_EN      (1UL << 4UL)
#define SDHCI_CMD_CRC_CHK_EN        (1UL << 3UL)
#define SDHCI_SUB_CMD_FLAG          (1UL << 2UL)
#define SDHCI_RESP_TYPE_NONE        (0UL << 1UL)
#define SDHCI_RESP_TYPE_LEN_136     (1UL << 1UL)
#define SDHCI_RESP_TYPE_LEN_48      (2UL << 1UL)
#define SDHCI_RESP_TYPE_LEN_48_BSY  (3UL << 1UL)

#define SDHCI_ARGUMENT              0x008           // 32-bytes

#define SDHCI_RESPONSE              0x010           // 32-bytes
#define SDHCI_RESPONSE0             0x10  // bits 31:0
#define SDHCI_RESPONSE1             0x14  // bits 63:32
#define SDHCI_RESPONSE2             0x18  // bits 95:64
#define SDHCI_RESPONSE3             0x1C  // bits 127:96

#define SDHCI_BUFFER                0x020           // 32-bytes

#define SDHCI_PRESENT               0x024           // 32-bytes
#define SDHCI_UHS2                  (1UL << 31UL)
#define SDHCI_LANE_SYNC             (1UL << 30UL)
#define SDHCI_DORMANT               (1UL << 29UL)
#define SDHCI_SUB_CMD_STATUS        (1UL << 28UL)
#define SDHCI_CMD_NOT_ISSUED        (1UL << 27UL)
#define SDHCI_HOST_VOLTAGE_STABLE   (1UL << 25UL)
#define SDHCI_LINE_SIGNAL_LEVEL     (1UL << 20UL)
#define SDHCI_WRITE_PROTECT_PIN     (1UL << 19UL)
#define SDHCI_CARD_DETECT_PIN       (1UL << 18UL)
#define SDHCI_CARD_STATUS_STABLE    (1UL << 17UL)
#define SDHCI_CARD_INSERTED         (1UL << 16UL)
#define SDHCI_BUFFER_READ_EN        (1UL << 11UL)
#define SDHCI_BUFFER_WRITE_EN       (1UL << 10UL)
#define SDHCI_READ_TRANSFER_ACTIVE  (1UL << 9UL)
#define SDHCI_WRITE_TRANSFER_ACTIVE (1UL << 8UL)
#define SDHCI_RETUNE_REQUEST        (1UL << 3UL)
#define SDHCI_DAT_LINE_ACTIVE       (1UL << 2UL)
#define SDHCI_DAT_INHIBIT           (1UL << 1UL)
#define SDHCI_CMD_INHIBIT           (1UL << 0UL)

#define SDHCI_HOST_CONTROL                  0x028           // 8-bytes
#define SDHCI_CARD_DETECT_TEST              (1UL << 7UL)
#define SDHCI_CARD_DETECT_TEST_LEVEL        (1UL << 6UL)
#define SDHCI_EXTEND_DATA_TRANSFER_WIDTH    (1UL << 5UL)
#define SDHCI_DMA_SELECT(x)                 ((x) << 3UL)
#define SDHCI_SDMA      0
#define SDHCI_32_BIT    2
#define SDHCI_64_BIT    3
#define SDHCI_HIGH_SPEED_EN                 (1UL << 2UL)
#define SDHCI_DATA_TRANFER_WIDTH_4BIT       (1UL << 1UL)
#define SDHCI_LED_CONTROL                   (1UL << 0UL)

#define SDHCI_POWER_CONTROL             0x029           // 8-bytes
#define SDHCI_SDBUS_VOLTAGE_VDD2_1_8    (5UL << 5UL)
#define SDHCI_SDBUS_POWER_VDD2          (1UL << 4UL)
#define SDHCI_SDBUS_VOLTAGE_VDD1_3_3    (7UL << 1UL)
#define SDHCI_SDBUS_VOLTAGE_VDD1_3_0    (6UL << 1UL)
#define SDHCI_SDBUS_VOLTAGE_VDD1_1_8    (5UL << 1UL)
#define SDHCI_SDBUS_POWER_VDD1          (1UL << 0UL)

#define SDHCI_BLOCK_GAP_CONTROL         0x02A           // 8-bytes
#define SDHCI_INTERRUPT_BLOCK_GAP       (1UL << 3UL)
#define SDHCI_READ_WAIT                 (1UL << 2UL)
#define SDHCI_CONTINUE_REQUEST          (1UL << 1UL)
#define SDHCI_STOP_GAP_REQUEST          (1UL << 0UL)

#define SDHCI_WAKEUP_CONTROL            0x02B           // 8-bytes
#define SDHCI_WAKEUP_SD_REMOVE_EN       (1UL << 2UL)
#define SDHCI_WAKEUP_SD_INSERT_EN       (1UL << 1UL)
#define SDHCI_WAKEUP_SD_INTERRUPT_EN    (1UL << 0UL)

#define SDHCI_CLOCK_CONTROL             0x02C           // 16-bytes
#define SDHCI_SDLCLK_FREQ_SEL(freq)     ((freq) << 8UL)
#define SDHCI_CLOCK_GEN_SEL             (1UL << 5UL)
#define SDHCI_PLL_EN                    (1UL << 3UL)
#define SDHCI_SD_CLOCK_EN               (1UL << 2UL)
#define SDHCI_INTERNAL_CLOCK_STABLE     (1UL << 1UL)
#define SDHCI_INTERNAL_CLOCK_EN         (1UL << 0UL)

#define SDHCI_TIMEOUT_CONTROL           0x02E           // 8-bytes
#define SDHCI_DATA_TIMEOUT_COUNTER(x)   ((x) << 0UL)

#define SDHCI_SOFTWARE_RESET            0x02F           // 8-bytes
#define SDHCI_RESET_DAT                 (1UL << 2UL)
#define SDHCI_RESET_CMD                 (1UL << 1UL)
#define SDHCI_RESET_ALL                 (1UL << 0UL)

#define SDHCI_INTERRUPT_STATUS          0x030           // 16-bytes
#define SDHCI_ERROR_INTERRUPT           (1UL << 15UL)
#define SDHCI_FX_EVENT                  (1UL << 13UL)
#define SDCHI_CARD_INTERRUPT            (1UL << 8UL)
#define SDHCI_CARD_REMOVAL              (1UL << 7UL)
#define SDHCI_BUFFER_READ_READY         (1UL << 5UL)
#define SDHCI_BUFFER_WRITE_READY        (1UL << 4UL)
#define SDHCI_DMA_INTERRUPT             (1UL << 4UL)
#define SDHCI_BLOCK_GAP_EVENT           (1UL << 2UL)
#define SDHCI_TRANSFER_COMPLETE         (1UL << 1UL)
#define SDHCI_CMD_COMPLETED             (1UL << 0UL)

#define SDHCI_ERROR_INTERRUPT_STATUS    0x032           // 16-bytes
#define SDHCI_RESPONSE_ERROR            (1UL << 11UL)
#define SDHCI_ADMA_ERROR                (1UL << 9UL)
#define SDHCI_AUTO_CMD_ERROR            (1UL << 8UL)
#define SDHCI_CURRENT_LIMIT_ERROR       (1UL << 7UL)
#define SDHCI_DATA_END_ERROR            (1UL << 6UL)
#define SDHCI_CRC_ERROR                 (1UL << 5UL)
#define SDHCI_DATA_TIMEOUT_ERROR        (1UL << 4UL)
#define SCHDI_CMD_INDEX_ERROR           (1UL << 3UL)
#define SDHCI_CMD_END_ERROR             (1UL << 2UL)
#define SDHCI_CMD_CRC_ERROR             (1UL << 1UL)
#define SDHCI_CMD_TIMEOUT_ERROR         (1UL << 0UL)

#define SDHCI_NORMAL_INTERRUPT_STATUS_EN       0x034       // 16-bytes
#define SDHCI_FX_EVENT_STATUS           (1UL << 13UL)
#define SDHCI_TUNING_EVENT_STATUS        (1UL << 12UL)
#define SDHCI_CARD_INTERRUPT_STATUS      (1UL << 8UL)
#define SDHCI_BUFFER_READ_STATUS        (1UL << 5UL)
#define SDHCI_BUFFER_WRITE_STATUS        (1UL << 4UL)
#define SDHCI_DMA_INTERRUPT_STATUS       (1UL << 3UL)
#define SDHCI_BLOCK_GAP_EVENT_STATUS     (1UL << 2UL)
#define SDHCI_TRANSFER_COMPLETE_STATUS   (1UL << 1UL)
#define SDHCI_CMD_COMPLETE_STATUS        (1UL << 0UL)

#define SDHCI_ERROR_INTERRUPT_STATUS_EN 0x036       // 16-bytes
#define SDHCI_RESPONSE_STATUS_ERROR_EN      (1UL << 11UL)
#define SDHCI_ADMA_ERROR_STATUS_EN          (1UL << 9UL)
#define SDHCI_AUTO_CMD_ERROR_STATUS_EN      (1UL << 8UL)
#define SDHCI_CURRENT_LIMIT_ERROR_STATUS_EN (1UL << 7UL)
#define SDHCI_DATA_BIT_ERROR_STATUS_EN      (1UL << 6UL)
#define SDHCI_DATA_CRC_ERROR_STATUS_EN      (1UL << 5UL)
#define SDHCI_DATA_TIMEOUT_ERROR_STATUS_EN  (1UL << 4UL)
#define SDHCI_CMD_INDEX_ERROR_STATUS_EN     (1UL << 3UL)
#define SDHCI_CMD_BIT_ERROR_STATUS_EN       (1UL << 2UL)
#define SDHCI_CMD_CRC_ERROR_STATUS_EN       (1UL << 1UL)
#define SDHCI_CMD_TIMEOUT_ERROR_STATUS_EN   (1UL << 0UL)

#define SDHCI_NORMAL_INTERRUPT_SIGNAL_EN    0x038           // 16-bytes
#define SDHCI_FX_EVENT_SIGNAL_EN            (1UL << 13UL)
#define SDHCI_CARD_INTERRUPT_SIGNAL_EN      (1UL << 8UL)
#define SDHCI_BUFFER_READ_SIGNAL_EN         (1UL << 5UL)
#define SDHCI_BUFFER_WRITE_SIGNAL_EN        (1UL << 4UL)
#define SDHCI_DMA_INTERRUPT_SIGNAL_EN       (1UL << 3UL)
#define SDHCI_BLOCK_GAP_SIGNAL_EN           (1UL << 2UL)
#define SDHCI_TRANFER_COMPLETE_SINGAL_EN    (1UL << 1UL)
#define SDHCI_CMD_COMPLETE_SIGNAL_EN        (1UL << 0UL)

#define SDHCI_ERROR_INTERRUPT_SIGNAL_EN     0x03A           // 16-bytes
#define SDHCI_RESP_ERROR_SIGNAL_EN          (1UL << 11UL)
#define SDHCI_ADMA_ERROR_SIGNAL_EN          (1UL << 9UL)
#define SDHCI_AUTO_CMD_ERROR_SIGNAL_EN      (1UL << 8UL)
#define SDHCI_CURRENT_LIMIT_ERROR_SIGNAL_EN (1UL << 7UL)
#define SDHCI_DATA_BIT_ERROR_SIGNAL_EN      (1UL << 6UL)
#define SDHCI_DATA_CRC_ERROR_SIGNAL_EN      (1UL << 5UL)
#define SDHCI_DATA_TIMEOUT_ERROR_SIGNAL_EN  (1UL << 4UL)
#define SDHCI_CMD_INDEX_SIGNAL_ERROR_EN     (1UL << 3UL)
#define SDHCI_CMD_END_SIGNAL_ERROR_EN       (1UL << 2UL)
#define SDHCI_CMD_CRC_SIGNAL_ERROR_EN       (1UL << 1UL)
#define SDHCI_CMD_TIMEOOUT_SIGNAL_ERROR_EN  (1UL << 0UL)

#define SDHCI_CMD_ERROR_STATUS              0x03C           // 16-bytes
#define SDHCI_CMD_NOT_ISSUED_ERROR          (1UL << 7UL)
#define SDHCI_AUTO_CMD_RESP_ERROR           (1UL << 5UL)
#define SDHCI_AUTO_CMD_INDEX_ERROR          (1UL << 4UL)
#define SDHCI_AUTO_CMD_END_ERROR            (1UL << 3UL)
#define SDHCI_AUTO_CMD_CRC_ERROR            (1UL << 2UL)
#define SDHCI_AUTO_CMD_TIMEOUT_ERROR        (1UL << 1UL)
#define SDHCI_AUTO_CMD12_NOT_EXEC           (1UL << 0UL)

#define SDHCI_HOST_CONTROL2                 0x03E           // 16-bytes
#define SDHCI_PRESENT_VALUE_EN              (1UL << 15UL)
#define SDHCI_ASYNC_INTERRUPT_EN            (1UL << 14UL)
#define SDHCI_64BIT_ADDRESSING              (1UL << 13UL)
#define SDHCI_HOST_V4_EN                    (1UL << 12UL)
#define SDHCI_CMD23_EN                      (1UL << 11UL)
#define SDHCI_ADMA2_LENGTH_MODE             (1UL << 10UL)

#define SDHCI_CAPABILITIES                  0x040           // 64-bytes
#define SDHCI_CAP_1_8_VDD2                  (1UL << 60UL)
#define SDHCI_CAP_ADMA3                     (1UL << 59UL)
#define SDHCI_CAP_CLOCK_MULTIPLIER          (1UL << 48UL)
#define SDHCI_CAP_SLOT_REMOVABLE            (0UL << 30UL)
#define SDHCI_CAP_SLOT_EMBEDDED             (1UL << 30UL)
#define SDHCI_CAP_SHARED_BUS_SLOT           (2UL << 30UL)
#define SDHCI_CAP_ASYNC_INTERRUPT           (1UL << 29UL)
#define SDHCI_CAP_64BIT_ADDRESSING_V3       (1UL << 28UL)
#define SDHCI_CAP_64BIT_ADDRESSING_V4       (1UL << 27UL)
#define SDHCI_CAP_VOLTAGE_1_8               (1UL << 26UL)
#define SDHCI_CAP_VOLTAGE_3_0               (1UL << 25UL)
#define SDHCI_CAP_VOLTAGE_3_3               (1UL << 24UL)
#define SDHCI_CAP_SUSPEND_RESUME            (1UL << 23UL)
#define SDHCI_CAP_SDMA                      (1UL << 22UL)
#define SDHCI_CAP_HIGH_SPEED                (1UL << 21UL)
#define SDHCI_CAP_ADMA2                     (1UL << 19UL)
#define SDHCI_CAP_8BIT                      (1UL << 18UL)
#define SDHCI_CAP_MAX_BLOCK_LEN_512         (0UL << 16UL)
#define SDHCI_CAP_MAX_BLOCK_LEN_1024        (1UL << 16UL)
#define SDHCI_CAP_MAX_BLOCK_LEN_2048        (2UL << 16UL)
#define SDHCI_CAP_TIMEOUT_CLOCK_UNIT_KHZ    (0UL << 7UL)
#define SDHCI_CAP_TIMEOUT_CLOCK_UNIT_MHZ    (1UL << 7UL)

#define SDHCI_ADMA_ERROR_STATUS                   0x054           // 16-bytes
#define ADMA_ERROR_LENGTH_MISMATCH          (1UL << 2UL)

#define SDHCI_ADMA_SYSTEM_ADDRESS                 0x058           // 32-bytes
#define ADMA_SYSTEM_ADMA2                   (2UL << 0UL)

#define ADMA2_VALID    (1 << 0)
#define ADMA2_END      (1 << 1)
#define ADMA2_INT      (1 << 2)
#define ADMA2_ACT1     (1 << 4) // Part of 0x2 (Transfer Data)
#define ADMA2_ACT2     (1 << 5)


// TODO: Get base clock from capabilites instead. Since this is QEMU, I won't worry
#define SDHCI_BASE_CLOCK        50000000
#define SDHCI_LOW_SPEED_MHZ     400000
#define SDHCI_HIGH_SPEED_MHZ    25000000

// SDHCI Cadence
#define SDHCI_CDNS_HRS00		0x00	/* Controller Software Reset / Version info */
#define  SDHCI_CDNS_HRS00_SWR           (1UL << 0UL)

#define SDHCI_CDNS_HRS01		0x04	/* Slot Control and DMA Settings */

#define SDHCI_CDNS_HRS04		0x10	/* PHY Memory Access Port (Crucial for high speed) */
#define  SDHCI_CDNS_HRS04_ADDR(x)       (((x) & 0x1F) << 16)
#define  SDHCI_CDNS_HRS04_WDATA(x)      (((x) & 0xFF) << 8)
#define  SDHCI_CDNS_HRS04_RDATA(x)      (((x) & 0xFF) << 0)
#define  SDHCI_CDNS_HRS04_RD            (1UL << 25UL)
#define  SDHCI_CDNS_HRS04_WR            (1UL << 24UL)
#define  SDHCI_CDNS_HRS04_ACK           (1UL << 26UL)

#define SDHCI_CDNS_HRS06		0x18	/* Mode Control Register */
#define  SDHCI_CDNS_HRS06_TUNE_UP       (1UL << 15UL)
#define  SDHCI_CDNS_HRS06_TUNE          (1UL << 14UL)
#define  SDHCI_CDNS_HRS06_MODE_MASK     0x7UL
#define  SDHCI_CDNS_HRS06_MODE_SD       0x0UL   /* Standard SD Card Mode */
#define  SDHCI_CDNS_HRS06_MODE_MMC_SDR  0x2UL   /* eMMC Single Data Rate */
#define  SDHCI_CDNS_HRS06_MODE_MMC_DDR  0x3UL   /* eMMC Dual Data Rate */














typedef struct {
    uint32_t    block_size;
    uint32_t    base_clock;
    uint16_t    rca;
    uint32_t    cid[16];
    uint32_t    csd[4];
} __attribute__((packed)) sdhci_t;

typedef struct {
    uint16_t    attributes;
    uint16_t    len;
    uint32_t    addr;
} __attribute__((packed, aligned(4))) sdhci_adma_desc_t;










void sdhci_init(void);
void sdhci_reset(void);
void sdhci_set_clock(uint32_t target);
void sdhci_irq_handler(void);
int sdhci_read_block(uint32_t lba, uint8_t *buf);
int sdhci_read_blocks(uint32_t lba, uint32_t length, uint8_t *buf);
int sdhci_dma_read(uint32_t lba, uint32_t length, uint8_t *buf);

#endif