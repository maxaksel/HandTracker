/**
 * Button input library.
 * @date 04/11/2022
 */

#include <msp430.h>
#include <stdbool.h>
#include "button.h"

bool button_event_flag;

/**
 * Button setup code
 */
void button_setup() {
    // Set up button inputs
    P1DIR &= ~(BIT3 + BIT4); // set to input
    P1REN |= BIT3 + BIT4; // enable pull up/down resistors
    P1OUT |= BIT3 + BIT4; // set resistors to pull up

    P1IES |= BIT3 + BIT4; // listen for high to low transitions (i.e., button presses)
    P1IFG &= ~(BIT3 + BIT4); // clear any pending interrupts
    P1IE |= BIT3 + BIT4; // enable interrupts for these pins

    // Set up internal state variables
    button_event_flag = false;
}

/**
 * Enable button input.
 */
void enable_button_input() {
    P1IE |= BIT3 + BIT4; // enable interrupts for these pins
    P1IES |= BIT3 + BIT4; // listen for high to low transitions (i.e., button presses)
}

/**
 * Disable button input.
 */
void disable_button_input() {
    P1IFG &= ~(BIT3 + BIT4); // clear interrupt bits
    P1IE |= BIT3 + BIT4; // enable interrupts for these pins
}

/**
 * Returns true if a button event (press or release) has occurred.
 *
 * @return a boolean.
 */
bool get_button_event_flag() {
    return button_event_flag;
}

/**
 * Clears button event flag.
 */
void clear_button_event_flag() {
    button_event_flag = false;
}

/**
 * Determines which button has been
 * pressed (1: SW1, 2: SW2). Returns
 * -1 if no button is depressed.
 */
int get_button_pressed() {
    bool sw1 = ~P1IN & BIT3;
    bool sw2 = ~P1IN & BIT4;

    if (sw1) return 1;
    else if (sw2) return 2;
    return 0; // if no button pressed
}

/**
 * Button interrupt service routine.
 * Sets values of inputs array according to button inputs.
 * Sets appropriate LED value. Add code here to modify ISR.
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void button(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) button (void)
#else
#error Compiler not supported!
#endif
{
    button_event_flag = true;
    P1IES ^= BIT3 + BIT4; // toggle edge Trigger
    P1IFG &= ~(BIT3 + BIT4); // clear interrupt bits
    // __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)
}
