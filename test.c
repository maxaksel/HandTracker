/*
 * Functions for testing various system components.
 *
 * @date 04/11/2022
 */

#include <msp430.h>

void flash_led(void);




void flash_led(void){
    P2DIR |= BIT5;
    volatile int i;

    while(1){
        P2OUT ^= BIT5; //toggle P2.5 (RED LED)
        for(i = 0; i < 100000; i++);
    }


}


