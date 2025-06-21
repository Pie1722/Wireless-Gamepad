// serial.h
#ifndef SERIAL_H
#define SERIAL_H

#include <avr/io.h>

void UART_init();
void UART_send_char(char data);
void UART_print(const char *str);

#endif
