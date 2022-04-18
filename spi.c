/*
 * spi.c
 *
 *  Created on: Apr 16, 2022
 *      Author: joshs
 */

#include "spi.h"


int spi_init(void){

    //set USCWRST
    UCB0CTL1 |= 0b11000000; //Use SMCLK, set USB0WRST
    UCB0CTL0 = 0b01101001; //3-wire SPI,  Bus Master, MSB first, clk passive high


    UCB0BR0 = 0b00000100; //set software divider to 4 (250 kHz clock)
    UCB0BR1 = 0b00000000;

    //configure ports


    P1SEL |= (BIT5 | BIT6 | BIT7);
    P1SEL2 |= (BIT5 | BIT6 | BIT7);

    P1DIR |= (BIT5 | BIT7); //set as output (MOSI, SCLK)
    P1DIR &= ~(BIT6); // set as an input (MISO)
    P2OUT |= (BIT3 + BIT0 + BIT1); //set to high
    P2DIR |= (BIT3 + BIT0 + BIT1); //set as output
    P2OUT |= (BIT3 + BIT0 + BIT1); //set to high


    UCB0CTL1 &= ~0b00000001; //Clear USB0WRST


    //enable interrupts

        //NOT YET

    return 0;
}

uint8_t spi_send_receive_byte(uint8_t byte){

    while( (UCB0STAT & 0x01) ); //wait while busy

    UCB0TXBUF = byte;

    while( (UCB0STAT & 0x01) ); //wait while busy

    return UCB0RXBUF; //read and return buffer value

}


uint16_t spi_send_receive_two_bytes(uint16_t data){

    uint16_t retval;

    while( (UCB0STAT & 0x01) ); //wait while busy

    retval = ( 0xFF & UCB0RXBUF); //clear data from RXBUF

    UCB0TXBUF = ((0xFF00 & data)>>8); //send MSBs


    while( !(IFG2 & BIT3) ); //wait for TXB flag

    UCB0TXBUF = (0x00FF & data); //send LSBs

    while( !(IFG2 & BIT2) ); //wait for RXB flag

    retval = ( 0xFF & UCB0RXBUF)<<8; //set msb's

    while( !(IFG2 & BIT2) ); //wait for RXB flag

    retval |= (0xFF & UCB0RXBUF); //set lsb's



    return retval; //read and return buffer value





}

int spi_send_receive_len(uint8_t *data_send, uint8_t *data_receive, int num_bytes){
    return 0;
}
