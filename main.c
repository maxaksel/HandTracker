#include <msp430.h> 
#include <stdint.h>
#include "test.h"
#include "spi.h"


/**
 * main.c
 */

uint16_t val;
uint8_t val_low;





int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	spi_init();

	P2OUT &= ~BIT3;

	val = spi_send_receive_two_bytes(0x8F00);
	val_low = (uint8_t) val & 0xFF;

	P2OUT |= BIT3;

	while(1);



	//flash_led();

	return 0;
}
