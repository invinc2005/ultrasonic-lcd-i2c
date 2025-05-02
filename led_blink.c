#include <reg51.h>

void delay() {
    int i, j;
    for (i = 0; i < 500; i++)
        for (j = 0; j < 100; j++);
}

void main() {
    while (1) {
        P1 = 0xFF;  // Turn ON all LEDs
        delay();
        P1 = 0x00;  // Turn OFF all LEDs
        delay();
    }
}
