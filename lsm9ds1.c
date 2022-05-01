/**
 * Module for communicating with the accelerometer and gyroscope
 * onboard the LSM9DS1 chip.
 *
 * @date 04/25/2022
 */

#include "lsm9ds1.h"

uint8_t gyro_data_send[] = {GYRO_OUT_ADDR | 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
volatile uint8_t gyro_data_recv[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t acc_data_send[] = {ACC_OUT_ADDR | 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
volatile uint8_t acc_data_recv[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t mag_data_send[] = {MAG_OUT_ADDR | 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
volatile uint8_t mag_data_recv[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

/**
 * Sets up LSM9DS1 to read accelerometer and gyro. Writing to CTRL_REG1_G enables
 * both the accelerometer and gyro. Sending 11011001 = 0xD9 to this register corresponds
 * to a 40 Hz low-pass filter and 952 Hz output data rate.
 */
void setup_acc_gyro() {
    spi_clk_passive_high();

    // Set up ACC and GYRO
    P2OUT &= ~CS_AG; // pull A/G chip select down

    uint8_t ctrl_send[] = {CTRL_REG1_G, 0xD9};
    uint8_t ctrl_recv[] = {0x0, 0x0};
    spi_start_asynch_transmission(ctrl_send, ctrl_recv, 2);
    while (!spi_free());

    P2OUT |= CS_AG; // pull A/G chip select up
}

void setup_mag() {
    // Set up MAG
    P2OUT &= ~CS_M; // pull M chip select down
    uint8_t ctrl_send[] = {CTRL_REG1_M, 0xB4};
    uint8_t ctrl_recv[] = {0x0, 0x0};
    spi_start_asynch_transmission(ctrl_send, ctrl_recv, 2);
    while (!spi_free());
    P2OUT |= CS_M;
    __delay_cycles(15);

    P2OUT &= ~CS_M; // pull M chip select down
    ctrl_send[0] = CTRL_REG2_M;
    ctrl_send[1] = 0x00;
    spi_start_asynch_transmission(ctrl_send, ctrl_recv, 2);
    while (!spi_free());
    P2OUT |= CS_M;
    __delay_cycles(15);

    P2OUT &= ~CS_M; // pull M chip select down
    ctrl_send[0] = CTRL_REG3_M;
    ctrl_send[1] = 0x00;
    spi_start_asynch_transmission(ctrl_send, ctrl_recv, 2);
    while (!spi_free());
    P2OUT |= CS_M;
    __delay_cycles(15);

    P2OUT &= ~CS_M; // pull M chip select down
    ctrl_send[0] = CTRL_REG4_M;
    ctrl_send[1] = 0x08;
    spi_start_asynch_transmission(ctrl_send, ctrl_recv, 2);
    while (!spi_free());
    P2OUT |= CS_M;
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
    __delay_cycles(15);
    spi_start_asynch_transmission(acc_data_send, acc_data_recv, 7);
    while (!spi_free());
    P2OUT |= CS_AG; // pull A/G chip select up
    __delay_cycles(15);
    P2OUT &= ~CS_AG; // pull A/G chip select down
    spi_start_asynch_transmission(gyro_data_send, gyro_data_recv, 7);
    while (!spi_free());

    // Read acc data
    data[0] = acc_data_recv[1];
    data[0] |= acc_data_recv[2] << 8;
    data[1] = acc_data_recv[3];
    data[1] |= acc_data_recv[4] << 8;
    data[2] = acc_data_recv[5];
    data[2] |= acc_data_recv[6] << 8;

    // Read gyro data
    data[3] = gyro_data_recv[1];
    data[3] |= gyro_data_recv[2] << 8;
    data[4] = gyro_data_recv[3];
    data[4] |= gyro_data_recv[4] << 8;
    data[5] = gyro_data_recv[5];
    data[5] |= gyro_data_recv[6] << 8;

    P2OUT |= CS_AG; // pull A/G chip select up
}

/**
 * Get magnetometer data from IMU and load it into a buffer of int16.
 *
 * @param data buffer.
 */
void get_mag(int16_t* data) {
    spi_clk_passive_high();

    P2OUT &= ~CS_M; // pull M chip select down

    while (!spi_free());
    __delay_cycles(15);
    spi_start_asynch_transmission(mag_data_send, mag_data_recv, 7);
    while (!spi_free());
    P2OUT |= CS_M; // pull M chip select up

    // Read magnetometer data
    data[0] = mag_data_recv[1];
    data[0] |= mag_data_recv[2] << 8;
    data[1] = mag_data_recv[3];
    data[1] |= mag_data_recv[4] << 8;
    data[2] = mag_data_recv[5];
    data[2] |= mag_data_recv[6] << 8;
}
