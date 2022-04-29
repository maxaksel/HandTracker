/**
 * Module for communicating with the accelerometer and gyroscope
 * onboard the LSM9DS1 chip.
 *
 * @date 04/25/2022
 */

#include "lsm9ds1.h"

uint8_t gyro_data_send[] = {GYRO_OUT_ADDR | 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t gyro_data_recv[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t acc_data_send[] = {ACC_OUT_ADDR | 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t acc_data_recv[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

/**
 * Sets up LSM9DS1 to read accelerometer and gyro. Writing to CTRL_REG1_G enables
 * both the accelerometer and gyro. Sending 11011001 = 0xD9 to this register corresponds
 * to a 40 Hz low-pass filter and 952 Hz output data rate.
 */
void setup_acc_gyro() {
    spi_clk_passive_high();

    P2OUT &= ~CS_AG; // pull A/G chip select down

    uint8_t ctrl_send[] = {CTRL_REG1_G, 0xD9};
    uint8_t ctrl_recv[] = {0x0, 0x0};
    spi_start_asynch_transmission(ctrl_send, ctrl_recv, 2);
    while (!spi_free());

    P2OUT |= CS_AG; // pull A/G chip select up
}

/**
 * Read accelerometer and gyroscope values into a 12-byte data buffer specified
 * as a parameter. This function is blocking.
 *
 * @param data an array of length six containing int16_t data
 */
void get_acc_gyro(int16_t* data) {
    spi_clk_passive_high();

    P2OUT &= ~CS_AG; // pull A/G chip select down

    while (!spi_free());
    spi_start_asynch_transmission(gyro_data_send, gyro_data_recv, 6);
    while (!spi_free());
    spi_start_asynch_transmission(acc_data_send, acc_data_recv, 6);
    while (!spi_free());

    // Read acc data
    data[0] = acc_data_recv[1] << 8;
    data[0] |= acc_data_recv[2];
    data[1] = acc_data_recv[3] << 8;
    data[1] |= acc_data_recv[4];
    data[2] = acc_data_recv[5] << 8;
    data[2] |= acc_data_recv[6];

    // Read gyro data
    data[3] = gyro_data_recv[1] << 8;
    data[3] |= gyro_data_recv[2];
    data[4] = gyro_data_recv[3] << 8;
    data[4] |= gyro_data_recv[4];
    data[5] = gyro_data_recv[5] << 8;
    data[5] |= gyro_data_recv[6];

    P2OUT |= CS_AG; // pull A/G chip select up
}
