/*
 * SPI.h
 *
 *  Created on: Apr 16, 2022
 *      Author: joshs
 */

#include <stdint.h>
#include <msp430.h>


#ifndef SPI_H_
#define SPI_H_

int spi_init(void);

uint8_t spi_send_receive_byte(uint8_t byte);

uint16_t spi_send_receive_two_bytes(uint16_t data);

int spi_send_receive_len(uint8_t *data_send, uint8_t *data_receive, int num_bytes);




#endif /* SPI_H_ */
