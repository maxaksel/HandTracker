/**
 * Module for polling sensors at a specific
 * frequency.
 *
 * @date 04/11/2022
 */

#include <msp430.h>
#include <stdbool.h>
#include "timera0.h"

/**
 * Timer A0 Module Status Variables
 */
unsigned int a0_divider;
unsigned int a0_div_tracker;
bool a0_flag;
bool is_paused;

/**
 * Set up A0 timer module in continuous mode with SMCLK as a trigger.
 */
void a0_setup() {
    TACTL = TASSEL_2 + MC_2;
}

/**
 * Start A0 timer with a given frequency and divider. If the frequency
 * desired is smaller than 16, a divider must be used.
 *
 * @param frequency an unsigned integer greater than or equal to 16
 * @param divider an unsigned integer greater than or equal to 1
 */
void a0_start(unsigned int frequency, unsigned int divider) {
    CCTL0 = CCIE; // Timer A interrupt enabled (CCTL0 is TACCTL0)
    if (frequency < 16) {
        return; // CCR0 cannot be increased large enough to accommodate frequencies smaller than 16
    }
    a0_divider = divider;
    CCR0 = 1000000 / frequency; // 1 MHz -> CRR0 = 1
}

/**
 * Pause the timer (interrupt still triggers).
 */
void a0_pause() {
    is_paused = true;
}

/**
 * Resume the timer (interrupt still triggers).
 */
void a0_resume() {
    is_paused = false;
}

/**
 * Reset the timer to operate with a different frequency and divider.
 * Timer A interrupt is disabled during the reset process.
 */
void a0_reset(unsigned int frequency, unsigned int divider) {
    CCTL0 &= ~CCIE; // Timer A interrupt disabled (CCTL0 is TACCTL0)
    a0_start(frequency, divider);
}

/**
 * Stop the timer (interrupts disabled).
 */
void a0_stop() {
    CCTL0 &= ~CCIE; // Timer A interrupt disabled (CCTL0 is TACCTL0)
}

/**
 * Get Timer A trigger flag. True if triggered, false otherwise. User
 * is responsible for clearing the flag.
 */
bool get_a0_flag() {
    return a0_flag;
}

/**
 * Clear Timer A flag.
 */
void clear_a0_flag() {
    a0_flag = false;
}

/**
 * Timer A0 interrupt service routine.
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    if (is_paused) {
        return;
    }

    a0_div_tracker++;
    if (a0_div_tracker == a0_divider) {
        a0_flag = true;
        a0_div_tracker = 0;
    }
}
