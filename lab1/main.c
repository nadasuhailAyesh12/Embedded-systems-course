#include <LPC11xx.h>
#include <stdint.h>

#define GPIO_PORT0_DIR (*((volatile uint32_t *)0x50008000))  // Direction register for Port 0
#define GPIO_PORT0_DATA (*((volatile uint32_t *)0x50003FFC)) // Data register for Port 0
#define GPIO_PORT2_DIR (*((volatile uint32_t *)0x50028000))   // Direction register for Port 2
#define GPIO_PORT2_DATA (*((volatile uint32_t *)0x50023FFC)) // Data register for Port 2

int main() {
    // Setup phase
    GPIO_PORT0_DIR &= ~0b110; // Configure P0.1 and P0.2 as input (buttons)
    GPIO_PORT2_DIR |= 0b1111;  // Configure P2.0 to P2.3 as output (LEDs)
    GPIO_PORT2_DATA &= ~0b1111; // Initialize LEDs to off

    while (1) {
        // Check if Button 1 (P0.1) is pressed
        if (GPIO_PORT0_DATA & 0b10) {
            GPIO_PORT2_DATA |= 0b1111; // Set bits 0 to 3 (turn on LEDs)
            while (GPIO_PORT0_DATA & 0b10); // Wait for Button 1 to be released
        }

        // Check if Button 2 (P0.2) is pressed
        if (GPIO_PORT0_DATA & 0b100) { // Button 2 is pressed
            GPIO_PORT2_DATA &= ~0b1111; // Clear bits 0 to 3 (turn off LEDs)
            while (GPIO_PORT0_DATA & 0b100); // Wait for Button 2 to be released
        }
    }

    return 0;
}
