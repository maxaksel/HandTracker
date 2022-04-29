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
#include "resistor.h"


/**
 * main.c
 */

int val;
uint8_t retval;

uint8_t packet[20];





int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	//setup SPI
	spi_init();
	//set GIE
	__bis_SR_register(GIE);
	//setup ADC
	setup_resistors();
	//setup LSM
	setup_acc_gyro();
	//setup UART
	uart_init();
	//setup LEDs
	led_setup();
	//setup buttons
	button_setup();
	//request first bytes from ADC
	request_resistor_data();
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
	    get_acc_gyro((int16_t *) &packet[2]);
	    //read data from ADC
	    read_resistor_data((int8_t *) &packet[14]);
	    //request new data from ADC
	    request_resistor_data();
	    //get buttons
	    packet[19] = (uint8_t) get_button_pressed();
	    //send packet to host
	    uart_send_bytes(packet, 20);
	}
}
