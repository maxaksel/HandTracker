/*
 * Library for interfacing with LSM9DS1 accelerometer and gyro.
 *
 * @date 04/22/2022
 */

#include <msp430.h>
#include <stdint.h>

#define CS_AG BIT3
#define CHIP_ADDR 0x6b
#define WHO_AM_I 0x0f
#define GYRO_OUT_ADDR 0x18
#define ACC_OUT_ADDR 0x28

/**
 * Get axis rotation and acceleration acceleration rates along the x-, y-, and z-axes.
 * Acceleration in each direction is returned as a twos complement integer ranging from
 * -32,768 to +32,767. This range corresponds linearly to [-4g, +4g]. Angular rotation
 * in each direction is returned as a twos complement integer ranging from
 * -32,768 to +32,767. This range corresponds linearly to [-2000dps, 2000dps].
 *
 * @return a pointer to an array of six 16-bit signed integers.
 */
void read_gyro_acc_data(uint16_t*);
