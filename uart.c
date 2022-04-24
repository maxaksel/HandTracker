/*
 * uart.c
 *
 *  Created on: Apr 23, 2022
 *      Author: joshs
 */

#include <uart.h>

static uint8_t tx_buffer[UART_BUFFER_DEPTH];

static uint8_t rx_buffer[UART_BUFFER_DEPTH];

static int tx_level;

static int rx_level;

static uint8_t *tx_top;

static uint8_t *rx_top;






/**
 * Requires:
 *  Nothing
 *
 *  Effects:
 *      Initializes software/hardware resources for software buffered uart module on UCA0
 *
 */
void uart_init(){

    //initialize software buffers
    tx_level = 0;
    rx_level = 0;

    tx_top = tx_buffer;
    rx_top = rx_buffer;

    //initialize uart module

    UCA0CTL1 = 0b11110000; //UART settings
    UCA0CTL0 = 0b00100000; //more UART settings

    //Baud rate = 111 kHz
    UCA0BR0 = 0x09;
    UCA0BR1 = 0x00;



    //initialize pins as input



    //select alternate function

    //clear interrupts
    IFG2 &= ~(BIT0 + BIT1);

    //enable interrupts
    IE2 |= (BIT0 + BIT1);



}


/**
 * Requires:
 *  Nothing
 *
 * Effects:
 *  Returns the number of bytes in the uart transmit buffer
 */
int uart_tx_buffer_level(){
    return tx_level;
}

/**
 * Requires:
 *  Nothing
 *
 * Effects:
 *  Returns the number of bytes in the uart receive buffer
 */
int uart_rx_buffer_level(){
    return rx_level;
}

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
 *  Nothing
 *
 * Effects:
 *  Pauses uart transmission (data to host)
 *
 */
void uart_pause_transmission();


/**
 * Requires:
 *  Nothing
 *
 *  Effects:
 *      Pauses uart reception (new data rejected)
 */
void uart_pause_receive();



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


