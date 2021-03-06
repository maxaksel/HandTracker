/*
 * spi.c
 *
 *  Created on: Apr 16, 2022
 *      Author: joshs
 */

#include "spi.h"


bool spi_free(void){
    return async_done;
}

bool spi_overflow_occurred(void){
    bool temp = overflow;
    overflow = false;
    return temp; //return prev value of overflow
}

int spi_init(void){

    //set USCWRST
    UCB0CTL1 |= 0b11000000; //Use SMCLK
    UCB0CTL0 = 0b01101001; //3-wire SPI,  Bus Master, MSB first, clk passive high


    UCB0BR0 = 0b0010100; //set software divider to 20 (50.0 kHz clock)
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

    IE2 &= ~(BIT2 + BIT3); //disable TX/RX interrupt by default

    async_done = true;
    overflow = false;
    tx_loc = 0;
    rx_loc = 0;
    data_size = 0;

    __bis_SR_register(GIE); //enable interrupts

    return 0;
}



void spi_clk_passive_high(void){
    UCB0CTL0  |= 0b01000000; //set to active low
    UCB0CTL0 &= ~0b10000000; //change phase too
}




void spi_clk_passive_low(void){
    UCB0CTL0 &= ~0b01000000; //set clock to be active high
    UCB0CTL0 |= 0b10000000; //change phase too
}


int spi_send_receive_byte(uint8_t byte){

    if(async_done == false) //in middle of async transmission
        return -1;

    IE2 &= ~(BIT2 + BIT3); //disable interrupts for synchronous function

    while( (UCB0STAT & 0x01) ); //wait while busy

    UCB0TXBUF = byte;

    while( (UCB0STAT & 0x01) ); //wait while busy

    return UCB0RXBUF; //read and return buffer value

}


int spi_send_receive_two_bytes(uint16_t data){

    if(async_done == false) //in middle of async transmission
        return -1;

    IE2 &= ~(BIT2 + BIT3); //disable interrupts for synchronous function

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
    int i; //counter

    if(async_done == false) //in middle of async transmission
        return -1;

    IE2 &= ~(BIT2 + BIT3); //disable interrupts for synchronous function

    while( (UCB0STAT & 0x01) ); //wait until not busy

    (void) (UCB0RXBUF); //clear RX buffer

    UCB0TXBUF = *data_send;

    for(i = 0; i < num_bytes; i++){
        while(!(IFG2 & BIT3)); //wait for TXB flag
        if(i != num_bytes - 1){
            UCB0TXBUF = data_send[i]; //send ith byte
        }
        while(!(IFG2 & BIT2)); //wait for RXB flag
        data_receive[i] = 0xFF & UCB0RXBUF; //read byte to buffer
    }

    return 0;

}


int spi_start_asynch_transmission(uint8_t *data_send, uint8_t *data_receive, int num_bytes){

    if((tx_loc != data_size) || (rx_loc != data_size) || !async_done)
        return -1; //UCB0 busy with another transmission

    (void) (UCB0RXBUF); //clear RX buffer
    (void) (UCB0RXBUF); //clear RX buffer

    while( (UCB0STAT & 0x01) ); //wait until not busy

    async_done = false; //indicate that transmission has started but not yet completed

    // initialize internal veriables
    txbuf = data_send;
    rxbuf = data_receive;
    tx_loc = 0;
    rx_loc = 0;
    data_size = num_bytes;

    IFG2 &= ~(BIT2 + BIT3); //clear interrupt flags
    IE2 |= (BIT2 + BIT3); //enable interrupts

    UCB0TXBUF = txbuf[tx_loc++]; //put first byte in queue
    (void) (UCB0RXBUF); //clear RX buffer
    __bis_SR_register(GIE); //set GIE

    return 0; //successfully initialized transmission
}





