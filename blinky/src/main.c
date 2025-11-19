#include <stdint.h>

/*
 * REG32(addr) lets us treat a 32-bit memory address as a
 * read/write register.
 *
 * 'volatile' tells the compiler:
 *   "Don't cache this, don't optimize away reads/writes.
 *    Hardware can change this behind your back."
 */
#define REG32(addr) (*(volatile uint32_t *)(addr))

/*
 * Base address for GPIO Port 0 on nRF52840.
 * This comes from the datasheet / product specification.
 */
#define NRF_P0_BASE 0x50000000UL

/*
 * P0_OUT and P0_DIR are specific registers inside the GPIO block.
 * Their offsets (0x504 and 0x514) also come from the datasheet.
 *
 * Final addresses:
 *   P0_OUT -> 0x50000000 + 0x504 = 0x50000504
 *   P0_DIR -> 0x50000000 + 0x514 = 0x50000514
 */
#define P0_OUT REG32(NRF_P0_BASE + 0x504)
#define P0_DIR REG32(NRF_P0_BASE + 0x514)

#define LED_PIN 17

static void delay(volatile uint32_t ctr) {
  while (ctr--) {
    __asm__ volatile("nop");
  }
}

// testing
int main(void) {
  // Make test pin an output
  P0_DIR |= (1u << LED_PIN);

  // Drive it low (active-low LEDs turn ON)
  P0_OUT &= ~(1u << LED_PIN);

  while (1) {
  }
}

// int main(void) {
//
//   P0_DIR |= (1u << LED_PIN);
//
//   while (1) {
//     // Set PIN HIGH
//     P0_OUT |= (1u << LED_PIN);
//
//     // Delay
//     delay(100000);
//
//     P0_OUT &= ~(1u << LED_PIN);
//
//     // Delay
//     delay(100000);
//   }
// }
