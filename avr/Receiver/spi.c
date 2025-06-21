#include "spi.h"

void SPI_init(void) {
    DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2); // MOSI, SCK, CSN
    DDRB &= ~(1 << PB4); // MISO
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); // SPI enable, master
}

uint8_t SPI_transfer(uint8_t data) {
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}
