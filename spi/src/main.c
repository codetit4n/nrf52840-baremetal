#include <stdint.h>

#define REG32(addr) (*(volatile uint32_t*)(addr))

#define NRF_SPIM0_BASE 0x40003000UL
#define NRF_P0_BASE 0x50000000UL

#define SPIM0_CONFIG REG32(NRF_SPIM0_BASE + 0x554)
#define SPIM0_ENABLE REG32(NRF_SPIM0_BASE + 0x500)
#define SPIM0_FREQUENCY REG32(NRF_SPIM0_BASE + 0x524)
#define SPIM0_TASKS_START REG32(NRF_SPIM0_BASE + 0x010)
#define SPIM0_EVENTS_END REG32(NRF_SPIM0_BASE + 0x118)
#define SPIM0_TXD_PTR REG32(NRF_SPIM0_BASE + 0x544)
#define SPIM0_TXD_MAXCNT REG32(NRF_SPIM0_BASE + 0x548)
#define SPIM0_RXD_PTR REG32(NRF_SPIM0_BASE + 0x534)
#define SPIM0_RXD_MAXCNT REG32(NRF_SPIM0_BASE + 0x538)

#define PSEL_SCK REG32(NRF_SPIM0_BASE + 0x508)
#define PSEL_MOSI REG32(NRF_SPIM0_BASE + 0x50C)
#define PSEL_MISO REG32(NRF_SPIM0_BASE + 0x510)

#define CSN_PIN 26
#define SCK_PIN 2
#define MOSI_PIN 27
#define MISO_PIN 30

// Assuming P0 pins only
#define P0_CNF(pin) REG32(NRF_P0_BASE + 0x700UL + 4UL * (pin))
#define P0_OUTSET REG32(NRF_P0_BASE + 0x508)
#define P0_OUTCLR REG32(NRF_P0_BASE + 0x50C)

static void csn_low() {
	P0_OUTCLR = (1 << CSN_PIN);
}

static void csn_high() {
	P0_OUTSET = (1 << CSN_PIN);
}

static void trigger_transfer() {
	SPIM0_TASKS_START = 1;
}

static void delay(volatile uint32_t ctr) {
	while (ctr--) {
		__asm__ volatile("nop");
	}
}

static void test_spi(void) {

	static uint8_t buffer[] = {0xAA, 0x55, 0xAA, 0x55};

	// 1. Clear END event
	SPIM0_EVENTS_END = 0;

	// 2. Setup TX only
	SPIM0_TXD_PTR = (uint32_t)buffer;
	SPIM0_TXD_MAXCNT = sizeof(buffer);

	SPIM0_RXD_PTR = 0;
	SPIM0_RXD_MAXCNT = 0;

	// 3. Assert CS
	csn_low();
	delay(1000); // small visible setup gap

	// 4. Start SPI transfer
	SPIM0_TASKS_START = 1;

	// 5. Wait until transfer finishes
	while (SPIM0_EVENTS_END == 0) {
		/* wait */
	}

	delay(1000); // allow clocks to finish cleanly

	// 6. Deassert CS
	csn_high();

	// 7. Big gap between transactions
	delay(500000);
}

static void init_spi() {

	// NOTE: There is no need to do PSEL_CSN here, since CSN will be
	// controlled manually
	P0_CNF(CSN_PIN) = (1 << 0); // simplified
	csn_high();

	// CONFIG: SPIMODE = 0 | CPHA = 0 | CPOL = 0 | MSB first
	SPIM0_CONFIG = 0;

	// FREQUENCY = 125 kbps (for testing)
	SPIM0_FREQUENCY = 0x02000000UL;

	// PSEL_SCK
	PSEL_SCK = SCK_PIN; // simplified
	// Expanded as:
	// PSEL_SCK =
	// (0 << 31) | // CONNECT = connected
	// (0 << 5) |  // PORT = 0
	// (SCK_PIN << 0);  // PIN = SCK_PIN
	P0_CNF(SCK_PIN) = (1 << 0); // simplified
	// Expanded into:
	// P0_CNF(SCK_PIN) = 0; // DIR=0, INPUT=0, PULL=0, DRIVE=0, SENSE=0
	// P0_CNF(SCK_PIN) |= (1 << 0); // DIR = output (1)
	// P0_CNF(SCK_PIN) |= (0 << 1); // INPUT = connected (0)
	// P0_CNF(SCK_PIN) |= (0 << 2); // PULL = no pull (0)
	// P0_CNF(SCK_PIN) |= (0 << 8); // DRIVE = standard '0', standard '1' (0)
	// P0_CNF(SCK_PIN) |= (0 << 16); // SENSE = disabled (0)

	PSEL_MOSI = MOSI_PIN;	     // simplified
	P0_CNF(MOSI_PIN) = (1 << 0); // simplified

	PSEL_MISO = MISO_PIN; // simplified
	P0_CNF(MISO_PIN) = 0; // simplified

	SPIM0_ENABLE = 7;
}

int main(void) {

	init_spi();

	while (1) {
		test_spi();
	}
}
