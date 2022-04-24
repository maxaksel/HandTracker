/*
 * SPI.h
 *
 *  Created on: Apr 16, 2022
 *      Author: joshs
 */

#include <stdint.h>
#include <stdbool.h>
#include <msp430.h>


#ifndef SPI_H_
#define SPI_H_

/**
 * This value is set to true when an asynchronous spi transmission has been completed (all num_bytes sent/received)
 */
bool spi_free(void);


/**
 * Requires:
 *  Nothing
 *
 * Effects:
 *  This function initializes the UCB0 module to perform SPI data transmission at 250 kHz
 *
 *
 */
int spi_init(void);

/**
 * Requires:
 *  spi_init should have already been called, spi_free should return true before calling this
 *  chip select must be set active by another function
 *
 *  Effects:
 *      synchronously sends and receives byte over spi
 *
 *      This method holds the processor until completed, should only be used for setup
 *
 *      returns -1 on failure, data on success
 *
 */
int spi_send_receive_byte(uint8_t byte);


/**
 * Requires:
 *  spi_init should have already been called, spi_free should return true before calling this
 *  chip_select must be set active by another function
 *
 *  Effects:
 *      synchronously sends and receives two bytes over spi
 *
 *      This method holds the processor until completed, should only be used for setup
 *
 *      returns -1 on failure, data on success
 *
 */
int spi_send_receive_two_bytes(uint16_t data);



/**
 * Requires:
 *  spi_init should have already been called
 *  chip select should be pulled low by another function prior to using this
 *  data_send should be an array of num_bytes
 *  data_receive should be a location in memory that can store at least num_bytes bytes of data
 *  num_bytes should be greater than 0
 *
 * Effects:
 *   Synchronously send/receive num_bytes over SPI
 *
 *   This method holds the processor until completed, should only be used for setup
 *
 *     returns -1 on failure, 0 on success
 *
 */
int spi_send_receive_len(uint8_t *data_send, uint8_t *data_receive, int num_bytes);


/**
 * Requires:
 *  spi_init should have already been called, spi_free should return true before calling this
 *  chip_select must be set active by another function
 *  data_send and data_receive must point to valid regions in memory of at least num_bytes size each
 *  GIE should not be disabled for more than a few cycles during this period
 *
 *  Effects:
 *      initializes interrupt driven transmission and reception of num_bytes of data over spi
 *      spi_free will return true once transmission is complete
 *      returns 0 on success, -1 on failure
 *
 */
int spi_start_asynch_transmission(uint8_t *data_send, uint8_t *data_receive, int num_bytes);




#endif /* SPI_H_ */
