#ifndef UART_H
#define UART_H

#include <stdint.h>


#define UART_BASE 0x01000000

#define UART_DR     (*(volatile uint32_t *)(UART_BASE + 0x000))
#define UART_RSR    (*(volatile uint32_t *)(UART_BASE + 0x004))
#define UART_FR     (*(volatile uint32_t *)(UART_BASE + 0x018))
#define UART_ILPR   (*(volatile uint32_t *)(UART_BASE + 0x020))
#define UART_IBRD   (*(volatile uint32_t *)(UART_BASE + 0x024))
#define UART_FBRD   (*(volatile uint32_t *)(UART_BASE + 0x028))
#define UART_LCRH   (*(volatile uint32_t *)(UART_BASE + 0x02C))
#define UART_CR     (*(volatile uint32_t *)(UART_BASE + 0x030))
#define UART_IFLS   (*(volatile uint32_t *)(UART_BASE + 0x034))
#define UART_IMSC   (*(volatile uint32_t *)(UART_BASE + 0x038))
#define UART_RIS    (*(volatile uint32_t *)(UART_BASE + 0x03C))
#define UART_MIS    (*(volatile uint32_t *)(UART_BASE + 0x040))
#define UART_ICR    (*(volatile uint32_t *)(UART_BASE + 0x044))
#define UART_MACR   (*(volatile uint32_t *)(UART_BASE + 0x048))

// UART_FR flags
#define UART_TXFE   (1UL << 7UL)
#define UART_RXFF   (1UL << 6UL)
#define UART_TXFF   (1UL << 5UL)
#define UART_RXFE   (1UL << 4UL)
#define UART_BUSY   (1UL << 3UL)
#define UART_DCD    (1UL << 2UL)
#define UART_DSR    (1UL << 1UL)
#define UART_CTS    (1UL << 0UL)

// UART_LCRH flags
#define UART_SPS        (1UL << 7UL)
#define UART_WLEN_8BIT  (3UL << 5UL)
#define UART_WLEN_7BIT  (2UL << 5UL)
#define UART_WLEN_6BIT  (1UL << 5UL)
#define UART_WLEN_5BIT  (0UL << 5UL)
#define UART_FEN        (1UL << 4UL)
#define UART_STP2       (1UL << 3UL)
#define UART_EPS        (1UL << 2UL)
#define UART_PEN        (1UL << 1UL)
#define UART_BRK        (1UL << 0UL)

// UART IMSC flags
#define UART_OEIM   (1UL << 10UL)
#define UART_BEIM   (1UL << 9UL)
#define UART_PEIM   (1UL << 8UL)
#define UART_FEIM   (1UL << 7UL)
#define UART_RTIM   (1UL << 6UL)
#define UART_TXIM   (1UL << 5UL)
#define UART_RXIM   (1UL << 4UL)
#define UART_DSRMIM (1UL << 3UL)
#define UART_DCDMIM (1UL << 2UL)
#define UART_CTSMIM (1UL << 1UL)
#define UART_RIMIM  (1UL << 0UL)

// UART ICR flags
#define UART_RTIC   (1UL << 6UL)
#define UART_TXIC   (1UL << 5UL)
#define UART_RXIC   (1UL << 4UL)

// UART_CR flags
#define UART_RXE    (1UL << 9UL)
#define UART_TXE    (1UL << 8UL)
#define UART_EN     (1UL << 0UL)







void uart_init(void);
void uart_putc(char ch);
void uart_puts(const char *s);

#endif