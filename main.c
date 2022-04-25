#include <msp430.h> 
#include <stdint.h>
#include "serial.h"
#include "spi.h"
#include "uart.h"


/**
 * main.c
 */

int val;
uint8_t retval;





int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	

	/**THESE LINES TEST SPI
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

	P2OUT |= BIT3; //toggle CS
    **/

	//THESE LINES TEST UART

	uint8_t uart_send_data[7] = { 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x10};
	uint8_t uart_receive_data[2] = {0x00, 0x00};
	volatile int test_send;
	volatile int test_receive;

	uart_init();

	test_send = uart_send_bytes(uart_send_data, 7);

	test_receive = uart_receive_bytes(uart_receive_data, 2);

	while(1); //loop infinitely



	//flash_led();

	//return 0;
}
