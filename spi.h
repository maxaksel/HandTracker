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

bool async_done;


int spi_init(void);

uint8_t spi_send_receive_byte(uint8_t byte);

uint16_t spi_send_receive_two_bytes(uint16_t data);



/**
 * Synchronously send/receive data from SPI - CS must be pulled to active state outside of this function
 * data_send should be an array of num_bytes
 * data_receive should be a location in memory that can store at least num_bytes bytes of data
 * num_bytes should be greater than 0
 *
 * Remember that SPI sends + receives in full-duplex mode
 *  if only wanting to read for a specific byte, send all zeros for incative MOSI line
 *  if only wanting to send for a specific byte, ignore that bytes place in data_receive
 *
 */
int spi_send_receive_len(uint8_t *data_send, uint8_t *data_receive, int num_bytes);

int spi_start_asynch_transmission(uint8_t *data_send, uint8_t *data_receive, int num_bytes);




#endif /* SPI_H_ */
