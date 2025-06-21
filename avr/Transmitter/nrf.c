#include "nrf.h"

void nrf_write_register(uint8_t reg, uint8_t* data, uint8_t len) {
    CSN_LOW();
    SPI_transfer(W_REGISTER | reg);
    for (uint8_t i = 0; i < len; i++) {
        SPI_transfer(data[i]);
    }
    CSN_HIGH();
}

void nrf_write_command(uint8_t command) {
    CSN_LOW();
    SPI_transfer(command);
    CSN_HIGH();
}

void nrf_send_payload(uint8_t* data, uint8_t len) {
    nrf_write_command(FLUSH_TX);
    CSN_LOW();
    SPI_transfer(W_TX_PAYLOAD);
    for (uint8_t i = 0; i < len; i++) {
        SPI_transfer(data[i]);
    }
    CSN_HIGH();
    CE_HIGH();
    _delay_us(15);
    CE_LOW();
}

void nrf_init(void) {
    DDRB |= (1 << CE_PIN) | (1 << CSN_PIN);
    CE_LOW();
    CSN_HIGH();
    _delay_ms(100);

    uint8_t config = 0x0E;
    nrf_write_register(CONFIG, &config, 1);

    uint8_t en_aa = 0x01;
    nrf_write_register(EN_AA, &en_aa, 1);

    uint8_t en_rxaddr = 0x01;
    nrf_write_register(EN_RXADDR, &en_rxaddr, 1);

    uint8_t setup_retr = 0x1A;
    nrf_write_register(SETUP_RETR, &setup_retr, 1);

    uint8_t rf_ch = 76;
    nrf_write_register(RF_CH, &rf_ch, 1);

    uint8_t rf_setup = 0x0E;
    nrf_write_register(RF_SETUP, &rf_setup, 1);

    uint8_t tx_addr[5] = {'0','0','0','0','1'};
    nrf_write_register(TX_ADDR, tx_addr, 5);
    nrf_write_register(RX_ADDR_P0, tx_addr, 5);

    _delay_ms(2);
}
