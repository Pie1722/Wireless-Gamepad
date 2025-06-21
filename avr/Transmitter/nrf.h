#ifndef NRF_H
#define NRF_H

#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"

/* Define CE and CSN pins */
#define CE_PIN    PB1
#define CSN_PIN   PB2

/* Control macros */
#define CE_HIGH()    (PORTB |= (1 << CE_PIN))
#define CE_LOW()     (PORTB &= ~(1 << CE_PIN))
#define CSN_HIGH()   (PORTB |= (1 << CSN_PIN))
#define CSN_LOW()    (PORTB &= ~(1 << CSN_PIN))

/* nRF24L01+ Commands and Register Addresses */
#define W_REGISTER    0x20
#define R_REGISTER    0x00
#define W_TX_PAYLOAD  0xA0
#define FLUSH_TX      0xE1
#define CONFIG        0x00
#define EN_AA         0x01
#define EN_RXADDR     0x02
#define SETUP_RETR    0x04
#define RF_CH         0x05
#define RF_SETUP      0x06
#define STATUS        0x07
#define TX_ADDR       0x10
#define RX_ADDR_P0    0x0A

void nrf_init(void);
void nrf_write_register(uint8_t reg, uint8_t* data, uint8_t len);
void nrf_write_command(uint8_t command);
void nrf_send_payload(uint8_t* data, uint8_t len);

#endif
