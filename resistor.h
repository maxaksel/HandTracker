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
 * Set up resistor ADC to sample.
 */
void setup_resistors();

/**
 * Signals ADC to sample channels 0 through 4 inclusive. Once the
 * ADC has sampled these channels, output data can be read using the
 * read_resistor_data function.
 */
void request_resistor_data();

/**
 * Reads digitally sampled resistor values from the ADC after a data
 * request has been sent. Converts resolution from 12-bit to 8-bit
 * and loads each resistance value in a length-5 data buffer of int8_t
 * data type.
 *
 * @param a pointer to an int8_t buffer.
 */
void read_resistor_data(int8_t* data);

#endif
