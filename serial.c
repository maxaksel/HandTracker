/*
 * serial.c
 *
 *  Created on: Apr 24, 2022
 *      Author: joshs
 */

#include "serial.h"

//TXISR
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCIAB0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if((IFG2 & BIT3) && (IE2 & BIT3)){ //UCB0 TX Interrupt
        if(tx_loc != data_size){
            UCB0TXBUF = txbuf[tx_loc++]; //write byte to buffer
        }
        else{
            IE2 &= ~BIT3; //clear transmit interrupt enable
        }
    }
    if((IFG2 & BIT1) && (IE2 & BIT1)){ //UCA0 TX Interrupt
        if(tx_level != 0){
            UCA0TXBUF = *tx_top++;
            tx_level--; //decrement
            if(tx_top >= (tx_buffer + UART_BUFFER_DEPTH)){
                tx_top = tx_buffer; //loop back to bottom of buffer
            }
        }
        else{
            IE2 &= ~BIT1; //disable future uart interrupts
        }
    }
}


//RXISR
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCIAB0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if ((IFG2 & BIT2) && (IE2 & BIT2)){ //UCB0 RX Interrupt
        if(rx_loc != data_size){
            rxbuf[rx_loc++] = UCB0RXBUF; //read byte from buffer
            if(rx_loc == data_size){
                async_done = true; //indicate that transmission was completed
            }
        }
        else{
            (void) UCA0RXBUF; //clear IFG
        }
        if(UCB0STAT & BIT5){
            overflow = true;
        }
    }
    if((IFG2 & BIT0) && (IE2 & BIT0)){ //UCA0 RX Interrupt
        if(rx_level < UART_BUFFER_DEPTH){
            *rx_bottom++ = UCA0RXBUF;
            rx_level++;
            if(rx_bottom >= (rx_buffer + UART_BUFFER_DEPTH)){
                rx_bottom  = rx_buffer;
            }
        }
        else{
            (void) UCA0RXBUF; //clear IFG
        }
    }
}
