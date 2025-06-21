// serial.c
#include "serial.h"

void UART_init() {
    UBRR0H = (103 >> 8);
    UBRR0L = 103;
    UCSR0B = (1 << TXEN0);                        // Enable transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);       // 8-bit data, 1 stop bit
}

void UART_send_char(char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait until buffer empty
    UDR0 = data;
}

void UART_print(const char *str) {
    while (*str) {
        UART_send_char(*str++);
    }
}
