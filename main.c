#include <msp430.h> 
#include "led.h"
#include "button.h"


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	int led_on = 1;
	
	led_setup();
	button_setup();

	while (1) {
	    int button = get_button_pressed();
	    if (button == 1) {
	        led_on++;
	        if (led_on == 4) led_on = 1;
	    } else if (button == 2) {
	        led_on--;
	        if (led_on == 0) led_on = 3;
	    }

	    if (led_on == 1) {
	        d1_on();
	        d2_off();
	        d3_off();
	    } else if (led_on == 2) {
	        d1_off();
	        d2_on();
	        d3_off();
	    } else if (led_on == 3) {
	        d1_off();
	        d2_off();
	        d3_on();
	    }
	    while (get_button_pressed());
	    clear_button_event_flag();
	}
}
