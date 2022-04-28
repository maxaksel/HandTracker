/*
 * Resistor module. Interfaces with external SPI ADC
 * which is used to read the value of the finger resistors.
 *
 * @date 04/28/2022
 */

#include "resistor.h"
#include "spi.h"

#define NUM_RESISTORS 5

uint8_t setup_send[] = {0x68};
uint8_t setup_recv[] = {0x0};
uint8_t request_send[] = {0xA0};
uint8_t request_recv[] = {0x0};
uint8_t data_send[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t data_recv[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

/**
 * Set up resistor ADC to sample.
 */
void setup_resistors() {
    spi_clk_passive_low();
    P2OUT &= ~CS_A; // pull ADC chip-select low
    spi_start_asynch_transmission(setup_send, setup_recv, 1);
    P2OUT |= CS_A; // pull ADC chip-select high
}

/**
 * Signals ADC to sample channels 0 through 4 inclusive. Once the
 * ADC has sampled these channels, output data can be read using the
 * read_resistor_data function.
 */
void request_resistor_data() {
    spi_clk_passive_low();
    P2OUT &= ~CS_A; // pull ADC chip-select low
    spi_start_asynch_transmission(request_send, request_recv, 1);
    while (!spi_free());
    P2OUT |= CS_A; // pull ADC chip-select high
}

/**
 * Reads digitally sampled resistor values from the ADC after a data
 * request has been sent. Converts resolution from 12-bit to 8-bit
 * and loads each resistance value in a length-5 data buffer of int8_t
 * data type.
 *
 * @param a pointer to an int8_t buffer.
 */
void read_resistor_data(int8_t* data) {
    spi_clk_passive_low();
    P2OUT &= ~CS_A; // pull ADC chip-select low
    spi_start_asynch_transmission(data_send, data_recv, 10);
    while (!spi_free());
    P2OUT |= CS_A; // pull ADC chip-select high

    int i;
    uint16_t full_res_value;

    for (i = 0; i < NUM_RESISTORS; i++) {
        full_res_value = data[i << 1 + 1];
        full_res_value |= data[i << 1];
        data[i] = full_res_value >> 4; // convert from 12-bit to 8-bit resolution
    }
}
