#include <LPC11xx.h>
#include <stdint.h>

// Define GPIO registers
#define GPIO0DIR (*((volatile uint32_t *)0x50008000))  // Direction register for Port 0
#define GPIO0DATA (*((volatile uint32_t *)0x50003FFC)) // Data register for Port 0
#define GPIO1DIR (*((volatile uint32_t *)0x50010000))  // Direction register for Port 1 (Red LED)
#define GPIO1DATA (*((volatile uint32_t *)0x50013FFC)) // Data register for Port 1
#define GPIO2DIR (*((volatile uint32_t *)0x50028000))  // Direction register for Port 2
#define GPIO2DATA (*((volatile uint32_t *)0x50023FFC)) // Data register for Port 2

int seven_seg_encoder[] = {
    0xC0, // 0
    0xF9, // 1
    0xA4, // 2
    0xB0, // 3
    0x99, // 4
    0x92, // 5
    0x82, // 6
    0xF8, // 7
    0x80, // 8
    0x90  // 9
};

int main(void) {
    int num = 555;
    int i;

    GPIO2DIR |= 0xFF;
    GPIO1DIR |= 0x01;
    GPIO0DIR |= 0x0C;
    // Initialize the red LED to be off
    GPIO1DATA &= ~0x01; // Turn off the red LED (make sure it's initially off)

    while (1) {
        // Handle hundreds place
        GPIO0DATA = 0x04;  // Select the hundreds digit
        GPIO2DATA = seven_seg_encoder[num / 100]; // Display hundreds place
        for (i = 0; i < 5000; i++); // Delay

        // Handle tens place
        GPIO0DATA = 0x08;
        GPIO2DATA = seven_seg_encoder[(num / 10) % 10]; // Display tens place
        for (i = 0; i < 5000; i++); // Delay

        // Handle ones place
        GPIO0DATA = 0x10;  // Select the ones digit
        GPIO2DATA = seven_seg_encoder[num % 10]; // Display ones place
        for (i = 0; i < 5000; i++); // Delay

        if (num > 0) {
            num--;
        }

        // When the count reaches 0, turn on the red LED
        if (num == 0) {
            GPIO1DATA |= 0x01; // Turn on the red LED
        }
    }

    return 0;
}
