/*
 * uart.h
 *
 *  Created on: Apr 23, 2022
 *      Author: joshs
 */

#include <stdint.h>
#include <stdbool.h>
#include <msp430.h>

#ifndef UART_H_
#define UART_H_

//Depth of uart buffer in bytes
#define UART_BUFFER_DEPTH 32

uint8_t tx_buffer[UART_BUFFER_DEPTH]; //array to hold values being sent

uint8_t rx_buffer[UART_BUFFER_DEPTH]; //array to hold values being received

int tx_level; //how full transmit buffer is

int rx_level; //how full receive buffer is

uint8_t *tx_top; //read from top

uint8_t *rx_bottom;  //write to bottom

/**
 * Requires:
 *  Nothing
 *
 *  Effects:
 *      Initializes software/hardware resources for software buffered uart module on UCA0
 *
 */
void uart_init();




/**
 * Requires:
 *  Nothing
 *
 * Effects:
 *  Returns the number of bytes in the uart transmit buffer
 */
int uart_tx_buffer_level(); //get level of uart transmit buffer

/**
 * Requires:
 *  Nothing
 *
 * Effects:
 *  Returns the number of bytes in the uart receive buffer
 */
int uart_rx_buffer_level(); //get level of uart receive buffer

/**
 * Requires:
 *  Nothing
 *
 * Effects:
 *  Empties the uart transmit buffer
 *
 */
int uart_clear_tx_buffer();

/**
 * Requires:
 *  Nothing
 *
 *  Effects:
 *      Empties the uart receive buffer
 *
 */
int uart_clear_rx_buffer();



/**
 * Requires:
 *  num_bytes should be a non-negative integer
 *  data should point to a valid region of memory
 *
 *  Effects:
 *      adds upto num_bytes bytes of data from data into software transmit buffer
 *      returns the number of bytes added
 *
 *      returns less than num_bytes if the software buffer cannot hold all of data
 *
 */
int uart_send_bytes(uint8_t *data, int num_bytes);

/**
 * Requires:
 *  num_bytes should be a non-negative integer
 *  data should point to a valid region in memory of at least num_bytes
 *
 *  Effects:
 *      moves up to num_bytes from the software transmit buffer to data
 *      returns the number of bytes moved
 *
 *      returns less than num_bytes if num_bytes exceeds the number of bytes in the buffer
 *
 *
 */
int uart_receive_bytes(uint8_t *data, int num_bytes); //moves bytes from queue to data, returns number of bytes moved








#endif /* UART_H_ */
