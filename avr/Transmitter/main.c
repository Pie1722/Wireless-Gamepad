#include <avr/io.h>
#include <util/delay.h>
#include "serial.h"
#include "nrf.h"
#include "spi.h"

int main(void) {
    UART_init();
    SPI_init();
    nrf_init(); 

    while (1) {
        UART_print("Hello World!\r\n");
        _delay_ms(1000);
    }
}
