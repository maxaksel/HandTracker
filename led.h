/**
 * LED output library. Consumes P2.5, P2.4, and P2.2.
 *
 * @date 04/11/2022
 */
#include <stdbool.h>

#ifndef LED_H_
#define LED_H_

/**
 * Function prototypes.
 */
void led_setup();

void d1_toggle();
void d2_toggle();
void d3_toggle();

void d1_on();
void d2_on();
void d3_on();

void d1_off();
void d2_off();
void d3_off();
void all_off();

#endif
