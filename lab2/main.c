#include <LPC11xx.h>
#include <stdint.h>

// Define GPIO registers for controlling LEDs and 7-segment display
#define GPIO0DIR (*((volatile uint32_t *)0x50008000))  // Direction register for Port 0
#define GPIO0DATA (*((volatile uint32_t *)0x50003FFC)) // Data register for Port 0
#define GPIO1DIR (*((volatile uint32_t *)0x50010000))  // Direction register for Port 1 (Red LED)
#define GPIO1DATA (*((volatile uint32_t *)0x50013FFC)) // Data register for Port 1
#define GPIO2DIR (*((volatile uint32_t *)0x50028000))  // Direction register for Port 2
#define GPIO2DATA (*((volatile uint32_t *)0x50023FFC)) // Data register for Port 2

// Lookup table for 7-segment encoding (0-9)
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

// Interrupt handler for Timer32B0 (called when delay completes)
void TIMER32_0_IRQHandler(void) {
    if (LPC_TMR32B0->IR & 0x01) { // Check if match interrupt flag is set
        LPC_TMR32B0->IR = 0x01;  // Clear the interrupt flag
        delay_done = 1;          // Set delay completion flag
    }
}

// Initialize Timer32B0 for delays
void Timer32B0_Init(void) {
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 9);  // Enable clock for Timer32B0
    LPC_TMR32B0->TCR = 0x02;                // Reset Timer32B0
    LPC_TMR32B0->PR = 0;                    // No prescaler for timer ticks
    LPC_TMR32B0->MCR = 0x03;                // Interrupt and reset on match
    NVIC_EnableIRQ(TIMER_32_0_IRQn);        // Enable Timer32B0 interrupt
}

// Start Timer32B0 with a specified delay in milliseconds
void Timer32B0_Start(uint32_t ms) {
    LPC_TMR32B0->MR0 = ms * 48000; // Match value for ~1ms delay at 48 MHz clock
    LPC_TMR32B0->TCR = 0x01;       // Start the timer
    delay_done = 0;                // Clear delay completion flag
}

// Wait until the timer delay is complete
void Timer32B0_Wait(void) {
    while (!delay_done);           // Poll the delay completion flag
}

int main(void) {
    int num = 555; // Starting number to display on 7-segment

    // Configure GPIO directions
    GPIO2DIR |= 0xFF; // Set GPIO2 as output (7-segment display)
    GPIO1DIR |= 0x01; // Set GPIO1 as output (red LED)
    GPIO0DIR |= 0x0C; // Set GPIO0 as output (digit selector)

    // Initialize the red LED to be off
    GPIO1DATA &= ~0x01; // Turn off the red LED initially

    // Initialize Timer32B0 for delays
    Timer32B0_Init();

    while (1) {
        // Display the hundreds digit
        GPIO0DATA = 0x04;  // Activate the hundreds digit
        GPIO2DATA = seven_seg_encoder[num / 100]; // Encode and display the hundreds digit
        Timer32B0_Start(5); // Delay for 5ms
        Timer32B0_Wait();   // Wait for the delay to complete

        // Display the tens digit
        GPIO0DATA = 0x08;  // Activate the tens digit
        GPIO2DATA = seven_seg_encoder[(num / 10) % 10]; // Encode and display the tens digit
        Timer32B0_Start(5); // Delay for 5ms
        Timer32B0_Wait();   // Wait for the delay to complete

        // Display the ones digit
        GPIO0DATA = 0x10;  // Activate the ones digit
        GPIO2DATA = seven_seg_encoder[num % 10]; // Encode and display the ones digit
        Timer32B0_Start(5); // Delay for 5ms
        Timer32B0_Wait();   // Wait for the delay to complete

        // Decrement the number
        if (num > 0) {
            num--; // Decrement the number to count down
        }

        // Turn on the red LED when the count reaches zero
        if (num == 0) {
            GPIO1DATA |= 0x01; // Turn on the red LED
        }
    }

    return 0;
}
