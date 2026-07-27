#include <stddef.h>
#include <stdint.h>

#define REG32(addr) (*(volatile uint32_t*)(addr))

#define NRF_UARTE0_BASE 0x40002000UL
#define NRF_P0_BASE 0x50000000UL

// DK: UARTE0 default pins routed to J-Link VCOM
#define TX_PIN 6 // P0.06: UARTE0 TXD -> DK J-Link VCOM RX
#define RX_PIN 8 // P0.08: UARTE0 RXD <- DK J-Link VCOM TX (unused here)

#define P0_CNF(pin) REG32(NRF_P0_BASE + 0x700UL + 4UL * (pin))

#define ENABLE REG32(NRF_UARTE0_BASE + 0x500)
#define CONFIG REG32(NRF_UARTE0_BASE + 0x56C)
#define BAUDRATE REG32(NRF_UARTE0_BASE + 0x524)
#define PSEL_TXD REG32(NRF_UARTE0_BASE + 0x50C)
#define PSEL_RXD REG32(NRF_UARTE0_BASE + 0x514)

#define TXD_PTR REG32(NRF_UARTE0_BASE + 0x544)
#define TXD_MAXCNT REG32(NRF_UARTE0_BASE + 0x548)

#define TASKS_STARTTX REG32(NRF_UARTE0_BASE + 0x008)

#define EVENTS_ENDTX REG32(NRF_UARTE0_BASE + 0x120)
#define EVENTS_TXSTOPPED REG32(NRF_UARTE0_BASE + 0x158)

#define UART_TX_BUF_SIZE 256
static uint8_t tx_buf[UART_TX_BUF_SIZE];

static void delay(volatile uint32_t ctr) {
	while (ctr--) {
		__asm__ volatile("nop");
	}
}

static void init_uarte0(void) {
	ENABLE = 0; // Disable UARTE while configuring

	P0_CNF(TX_PIN) = (1 << 0) | // DIR = Output
			 (1 << 1) | // INPUT disconnect (input buffer not needed for TX)
			 (0 << 2) | // PULL = none (field)
			 (0 << 8) | // DRIVE = standard (field)
			 (0 << 16); // SENSE = disabled

	// PSEL format: PIN[4:0] | PORT(bit5) | CONNECT(bit31: 0=connected, 1=disconnected)
	PSEL_TXD = (TX_PIN << 0) | (0 << 5) | (0 << 31);

	PSEL_RXD = (1 << 31); // RX disconnected (TX-only)

	CONFIG = (0 << 0) |   // HWFC disabled
		 (0x0 << 1) | // PARITY excluded
		 (0 << 4);    // 1 stop bit

	BAUDRATE = 0x01D60000; // 115200 (per datasheet table)

	EVENTS_ENDTX = 0;
	EVENTS_TXSTOPPED = 0;

	ENABLE = 8; // Enable UARTE
}

static void tx_send(const uint8_t* tx, size_t len) {
	if (len == 0)
		return;
	if (len > 0xFFFF)
		len = 0xFFFF; // MAXCNT is 16-bit

	EVENTS_ENDTX = 0;
	// Set up TX buffer
	TXD_PTR = (uint32_t)(uintptr_t)tx;
	TXD_MAXCNT = len;

	// Start TX
	TASKS_STARTTX = 1;
	// Wait for TX to complete - blocking
	while (EVENTS_ENDTX == 0) {
	}
}

static void uarte_write(const char* text, size_t len) {
	if (len > sizeof(tx_buf))
		len = sizeof(tx_buf);

	for (size_t i = 0; i < len; i++)
		tx_buf[i] = (uint8_t)text[i];

	tx_send(tx_buf, len);
}

int main(void) {
	init_uarte0();

	while (1) {
		uarte_write("Hey UARTE!\r\n", 12);
		delay(10 * 1000 * 1000);
	}
}
