/**
 * Module for communicating with the accelerometer and gyroscope
 * onboard the LSM9DS1 chip.
 *
 * @date 04/25/2022
 */

#include <msp430.h>
#include <stdint.h>
#include "spi.h"

#define CS_AG BIT3
#define CS_M BIT1
#define CHIP_ADDR 0x6b
#define WHO_AM_I 0x0f
#define CTRL_REG1_G 0x10
#define CTRL_REG1_M 0x20
#define CTRL_REG2_M 0x21
#define CTRL_REG3_M 0x22
#define CTRL_REG4_M 0x23
#define GYRO_OUT_ADDR ((uint8_t) 0x18)
#define ACC_OUT_ADDR ((uint8_t) 0x28)
#define MAG_OUT_ADDR ((uint8_t) 0x28)

#ifndef LSM9DS1_H_
#define LSM9DS1_H_

/**
 * Sets up LSM9DS1 to read accelerometer and gyro. Writing to CTRL_REG1_G enables
 * both the accelerometer and gyro. Sending 11011001 = 0xD9 to this register corresponds
 * to a 40 Hz low-pass filter and 952 Hz output data rate.
 */
void setup_acc_gyro();

void setup_mag();

/**
 * Read accelerometer and gyroscope values into a 12-byte data buffer specified
 * as a parameter. This function is blocking.
 *
 * @param data an array of length six containing int16_t data
 */
void get_acc_gyro(int16_t* data);

void get_mag(int16_t* data);

#endif
