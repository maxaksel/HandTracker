#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>
#include "spi.h"
#include "uart.h"
#include "serial.h"
#include "timera0.h"
#include "lsm9ds1.h"
#include "led.h"
#include "button.h"


/**
 * main.c
 */

int val;
uint8_t retval;

uint8_t packet[20];
uint16_t gyro_data[6];





int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	//setup SPI
	spi_init();
	//set GIE
	__bis_SR_register(GIE);
	//setup ADC

	//setup LSM

	//setup UART
	uart_init();
	//setup LEDs
	led_setup();
	//setup buttons
	button_setup();
	//request first bytes from ADC

	//setup timer A for use with Interrupt
	a0_setup();
	//set to continuous running, 50 Hz
	a0_start(50); //run at 50 hz
	packet[0] = 0xCA; //packet header
    packet[1] = 0xFE; //packet header
	while (1) {
	    //wait for timer to go off
	    __bis_SR_register(GIE +  LPM0_bits); //enable interrupts and put in Sleep
	    //read data from LSM
	    //read_gyro_acc_data(gyro_data);
	    //transfer data to packet

	    //read data from ADC

	    //transfer to packet

	    //get buttons
	    packet[19] = (uint8_t) get_button_pressed();
	    //send packet to host
	    //uart_send_bytes(packet, 20);
	}
}
