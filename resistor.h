/**
 * Module for getting resistor values associated with each glove finger.
 *
 * @date 04/25/2022
 */

#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef RESISTOR_H_
#define RESISTOR_H_

#define CS_A BIT0

/**
 * Loads a five-byte buffer with resistor values. A pointer to the buffer is
 * specified as the parameter to this function.
 *
 * @param a uint8_t pointer.
 */
void get_resistor_data(uint8_t* data);

#endif
