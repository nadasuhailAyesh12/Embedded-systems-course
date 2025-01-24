#include <LPC11xx.h>
#include <stdint.h>

// GPIO Port Definitions
#define GPIO_PORT0_DIR (*((volatile uint32_t *)0x50008000))  // Direction register for Port 0
#define GPIO_PORT0_DATA (*((volatile uint32_t *)0x50003FFC)) // Data register for Port 0
#define GPIO_PORT2_DIR (*((volatile uint32_t *)0x50028000))  // Direction register for Port 2
#define GPIO_PORT2_DATA (*((volatile uint32_t *)0x50023FFC)) // Data register for Port 2

int main() {
    // Setup phase: Configure GPIO ports for inputs (buttons) and outputs (LEDs)

    // Configure P0.1 and P0.2 as input for buttons
    GPIO_PORT0_DIR &= ~0b110; // Set P0.1 and P0.2 as input (clearing bits 1 and 2)

    // Configure P2.0 to P2.3 as output for LEDs
    GPIO_PORT2_DIR |= 0b1111; // Set P2.0 to P2.3 as output (setting bits 0 to 3)

    // Initialize LEDs to OFF
    GPIO_PORT2_DATA &= ~0b1111; // Clear bits 0 to 3 to turn off LEDs

    while (1) {
        // Main loop: Check the button states and control LEDs accordingly

        // Check if Button 1 (connected to P0.1) is pressed
        if (GPIO_PORT0_DATA & 0b10) { // If P0.1 is high (button pressed)
            GPIO_PORT2_DATA |= 0b1111; // Turn on all LEDs (set bits 0 to 3)
            
            // Wait for Button 1 to be released before proceeding
            while (GPIO_PORT0_DATA & 0b10); // Busy-wait until P0.1 goes low
        }

        // Check if Button 2 (connected to P0.2) is pressed
        if (GPIO_PORT0_DATA & 0b100) { // If P0.2 is high (button pressed)
            GPIO_PORT2_DATA &= ~0b1111; // Turn off all LEDs (clear bits 0 to 3)
            
            // Wait for Button 2 to be released before proceeding
            while (GPIO_PORT0_DATA & 0b100); // Busy-wait until P0.2 goes low
        }
    }

    return 0; // This line is never reached in this program
}
