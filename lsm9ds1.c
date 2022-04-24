/**
 * Module for communicating with the LSM9DS1 sensor.
 *
 * @date 04/22/2022
 */

#include <msp430.h>
#include "lsm9ds1.h"

/**
 * Get axis rotation and acceleration acceleration rates along the x-, y-, and z-axes.
 * Acceleration in each direction is returned as a twos complement integer ranging from
 * -32,768 to +32,767. This range corresponds linearly to [-4g, +4g]. Angular rotation
 * in each direction is returned as a twos complement integer ranging from
 * -32,768 to +32,767. This range corresponds linearly to [-2000dps, 2000dps]. Assumes
 * SPI has already been configured and initialized.
 *
 * @return a pointer to an array of six 16-bit signed integers.
 */
void read_gyro_acc_data(uint16_t* retvals) {
    while (UCB0STAT & 0x01); // wait while busy
    uint8_t clear = UCB0RXBUF; // clear data from RXBUF

    P2OUT &= ~CS_AG; // pull accelerometer and gyro chip select low

    uint8_t request = 0x80 | GYRO_OUT_ADDR;
    UCB0TXBUF = request; // request data from the gyro register
    while (!(IFG2 & BIT3)); // wait for TXB flag

    unsigned char counter;
    for (counter = 0; counter < 3; counter++) {
        while(!(IFG2 & BIT2)); // wait for RXB flag
        retvals[counter] = UCB0RXBUF << 8; // set msbs
        while(!(IFG2 & BIT2)); // wait for RXB flag
        retvals[counter] |= UCB0RXBUF; // set lsbs
    }

    request = 0x80 | ACC_OUT_ADDR;
    UCB0TXBUF = request; // request data from the gyro register
    while (!(IFG2 & BIT3)); // wait for TXB flag

    for (counter = 3; counter < 6; counter++) {
        while(!(IFG2 & BIT2)); // wait for RXB flag
        retvals[counter] = UCB0RXBUF << 8; // set msbs
        while(!(IFG2 & BIT2)); // wait for RXB flag
        retvals[counter] |= UCB0RXBUF; // set lsbs
    }

    P2OUT |= CS_AG; // pull accelerometer and gyro chip select high
}
