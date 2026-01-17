#include <stdint.h>

#define REG32(addr) (*(volatile uint32_t*)(addr))

#define NRF_SPIM0_BASE 0x40003000UL
#define NRF_P0_BASE 0x50000000UL

#define SPIM0_CONFIG REG32(NRF_SPIM0_BASE + 0x554)
#define SPIM0_ENABLE REG32(NRF_SPIM0_BASE + 0x500)
#define SPIM0_FREQUENCY REG32(NRF_SPIM0_BASE + 0x524)

#define PSEL_SCK REG32(NRF_SPIM0_BASE + 0x508)
#define PSEL_MOSI REG32(NRF_SPIM0_BASE + 0x50C)
#define PSEL_MISO REG32(NRF_SPIM0_BASE + 0x510)

#define CSN_PIN 7
#define SCK_PIN 8
#define MOSI_PIN 9
#define MISO_PIN 10

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

	while (1)
		;
}
