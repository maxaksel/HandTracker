/*
 * uart.c
 *
 *  Created on: Apr 23, 2022
 *      Author: joshs
 */

#include "uart.h"
#include <msp430.h>

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
    rx_bottom = rx_buffer + UART_BUFFER_DEPTH - 1;

    //initialize uart module

    UCA0CTL1 = 0b11110001; //UART settings with WRST
    UCA0CTL0 = 0b00000000; //more UART settings

    //Baud rate = 38,400 kHz
    UCA0BR0 = 26;
    UCA0BR1 = 0x00;

    //Modulation settings
    UCA0MCTL = 0b00001100;

    //select alternate function

    P1SEL |= (BIT1 + BIT2);
    P1SEL2 |= (BIT1 + BIT2);

    //initialize pins as input

    P1DIR |= BIT2; //set TXD to output
    P1DIR &= ~BIT1; //set RXD to input

    //enable module
    UCA0CTL1 &= ~0b00000001; //clear wrst

    //enable interrupts
    IE2 |= (BIT0 + BIT1);

    __bis_SR_register(GIE); //enable global interrupts



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
int uart_clear_tx_buffer(){
    tx_level = 0;
    return 0;
}

/**
 * Requires:
 *  Nothing
 *
 *  Effects:
 *      Empties the uart receive buffer
 *
 */
int uart_clear_rx_buffer(){
    rx_level = 0;
    return 0;
}



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
int uart_send_bytes(uint8_t *data, int num_bytes){
    int i, bytes_add;
    //temporarily disable uart interrupts
    IE2 &= ~(BIT1 + BIT0); //disable interrupts before accessing tx_top & tx_level
    uint8_t *bottom = ((uint8_t *) tx_top) + tx_level;
    if (bottom >= (tx_buffer + UART_BUFFER_DEPTH))
        bottom -= UART_BUFFER_DEPTH;
    bytes_add = (num_bytes > (UART_BUFFER_DEPTH - tx_level)) ? (UART_BUFFER_DEPTH - tx_level) : num_bytes; //calculate bytes to add to buffer
    for (i = 0; i < bytes_add; i++){
        *bottom++ = data[i];
        if(bottom >= (tx_buffer + UART_BUFFER_DEPTH))
                bottom -= UART_BUFFER_DEPTH;
        tx_level++; //increment tx level
    }
    IE2 |= (BIT1 + BIT0); //enable interrupts
    if(bytes_add != num_bytes){
        __asm(" nop");
    }
    return bytes_add; //return number of bytes added
}


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
int uart_receive_bytes(uint8_t *data, int num_bytes){
    int i;
    IE2 &= ~(BIT1 + BIT0); //disable interrupts before accessing tx_top & tx_level
    unsigned int bytes_receive = (num_bytes > rx_level) ? (rx_level) : num_bytes;
    uint8_t *rx_top = ((uint8_t *) rx_bottom) - rx_level;
    IE2 |= (BIT1 + BIT0); //re-enable interrupts
    if(rx_top < rx_buffer)
        rx_top -= UART_BUFFER_DEPTH;

    for(i = 0; i < bytes_receive; i++){
        *data++ = *tx_top++;
         if(rx_top < rx_buffer)
             rx_top -= UART_BUFFER_DEPTH;
         rx_level--;
    }

    return bytes_receive;

}













