#include <msp430.h> 
#include <stdint.h>
#include "test.h"
#include "spi.h"


/**
 * main.c
 */

int val;
uint8_t retval;





int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	spi_init();

	P2OUT &= ~BIT3; //toggle CS

	uint8_t data_send_1[2] = {(0x00 | 0x10), 0b11000000};
	uint8_t data_receive_1[2] = {0x00, 0x00};

	uint8_t data_send_2[7] = {(0x80 | 0x18), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t data_receive_2[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


	while(!spi_free()); //wait until SPI module free

	spi_start_asynch_transmission(data_send_1, data_receive_1, 2);

	while(!spi_free()); //wait until SPI module free

	P2OUT |= BIT3; //toggle CS
	volatile int i;
	for(i = 0; i < 20000; i++);
	P2OUT &= ~BIT3; //toggle CS

	spi_start_asynch_transmission(data_send_2, data_receive_2, 7);

	while(!spi_free()); //wait until SPI module free

	//val = spi_send_receive_two_bytes(0x8F00);
	//retval = 0xFF & val;

	P2OUT |= BIT3; //toggle CS

	while(1); //loop infinitely



	//flash_led();

	return 0;
}
