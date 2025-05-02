#include <reg51.h>

void Timer0_Delay_1s();  // Function prototype for 1-second delay

void main() {
    unsigned char count = 0; // Counter variable
    P1 = 0x00;  // Initialize Port 1 to 0

    while (1) {
        P1 = count;  // Display count on Port 1
        count++;  // Increment count

        if (count > 60)  // Reset after reaching 60
            count = 0;

        Timer0_Delay_1s();  // 1-second delay using Timer 0
    }
}

void Timer0_Delay_1s() {
    unsigned int i;
    TMOD = 0x01;  // Timer0 Mode 1 (16-bit timer)

    for (i = 0; i < 60; i++) {  // 20 overflows for ~1 sec (assuming 11.0592 MHz clock)
        TH0 = 0x3C;  // Load Timer High byte (for 50ms delay)
        TL0 = 0xB0;  // Load Timer Low byte
        TR0 = 1;  // Start Timer 0

        while (TF0 == 0);  // Wait for Timer 0 overflow

        TR0 = 0;  // Stop Timer
        TF0 = 0;  // Clear overflow flag
    }
}
