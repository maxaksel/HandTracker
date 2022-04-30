/**
 * LED output library. Consumes P2.5, P2.4, and P2.2.
 *
 * @date 04/11/2022
 */

#include "led.h"
#include <msp430.h>

/**
 * Set up PORT2 register for LED output
 */
void led_setup() {
    P2DIR |= BIT5 + BIT4 + BIT2; // set P2.5, P2.4, and P2.2 as outputs
    P2OUT &= ~(BIT5 + BIT4 + BIT2); // clear P2.5, P2.4, and P2.2 bits (initially off)
}

/**
 * Toggle LED 1 (topmost LED with Micro USB facing down)
 */
inline void d1_toggle() {
    P2OUT ^= BIT5;
}

/**
 * Toggle LED 2 (middle LED with Micro USB facing down)
 */
inline void d2_toggle() {
    P2OUT ^= BIT4;
}

/**
 * Toggle LED 3 (bottom LED with Micro USB facing down)
 */
inline void d3_toggle() {
    P2OUT ^= BIT2;
}

/**
 * Turn on LED 1 (topmost LED with Micro USB facing down)
 */
inline void d1_on() {
    P2OUT |= BIT5;
}

/**
 * Turn on LED 2 (middle LED with Micro USB facing down)
 */
inline void d2_on() {
    P2OUT |= BIT4;
}

/**
 * Turn on LED 3 (bottom LED with Micro USB facing down)
 */
inline void d3_on() {
    P2OUT |= BIT2;
}

/**
 * Turn off LED 1 (topmost LED with Micro USB facing down)
 */
inline void d1_off() {
    P2OUT &= ~BIT5;
}

/**
 * Turn off LED 2 (middle LED with Micro USB facing down)
 */
inline void d2_off() {
    P2OUT &= ~BIT4;
}

/**
 * Turn off LED 3 (bottom LED with Micro USB facing down)
 */
inline void d3_off() {
    P2OUT &= ~BIT2;
}

/**
 * Turn off all LEDs
 */
inline void all_off() {
    P2OUT &= ~(BIT5 + BIT4 + BIT2); // clear P2.5, P2.4, and P2.2 bits
}
