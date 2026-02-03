#include <stddef.h>
#include <stdint.h>

#define REG32(addr) (*(volatile uint32_t*)(addr))

#define NRF_UARTE0_BASE 0x40002000UL
#define NRF_P0_BASE 0x50000000UL

// P0 only
#define TX_PIN 6 // P0.06 - DK default VCOM pins for RX
#define RX_PIN 8 // P0.08 - DK default VCOM pins for TX
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

// Hardcoded for demo purpose!
static uint8_t tx_buf[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x21}; // "Hello!"

static void delay(volatile uint32_t ctr) {
	while (ctr--) {
		__asm__ volatile("nop");
	}
}

static void init_uarte0(void) {

	ENABLE = 0; // Disable UARTE

	P0_CNF(TX_PIN) = (1 << 0) | // DIR = Output
			 (1 << 1) | // INPUT = Disconnect
			 (0 << 2) | // No pull
			 (0 << 8) | // Standard drive
			 (0 << 16); // No sense

	PSEL_TXD = (TX_PIN << 0) // pin select
		   | (0 << 5)	 // port 0
		   | (0 << 31);	 // connected - 0

	PSEL_RXD = (1 << 31); // disconnected

	CONFIG = (0 << 0)     // HWFC - 0 - disabled
		 | (0x0 << 1) // PARITY - 0x0 - exclude
		 | (0 << 4);  // STOP bits - 0 - one stop bit

	BAUDRATE = 0x01D60000;

	// clear events
	EVENTS_ENDTX = 0;
	EVENTS_TXSTOPPED = 0;

	ENABLE = 8; // Enable UARTE
}

static void tx_send(void) {
}

int main(void) {

	init_uarte0();

	while (1) {
	}
}
