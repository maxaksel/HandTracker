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

/**
 * Set up A0 timer module in continuous mode with SMCLK as a trigger.
 */
void a0_setup() {
    TACTL = TASSEL_2 + MC_2;
}

/**
 * Start A0 timer with a given frequency (Hz)
 *
 * @param frequency an unsigned integer greater than or equal to 16
 */
void a0_start(unsigned int frequency) {
    CCTL0 = CCIE; // Timer A interrupt enabled (CCTL0 is TACCTL0)
    if (frequency < 16) {
        return; // CCR0 cannot be increased large enough to accommodate frequencies smaller than 16
    }
    CCR0 = 1000000 / frequency; // 1 MHz -> CRR0 = 1
}

/**
 * Reset the timer to operate with a different frequency and divider.
 * Timer A interrupt is disabled during the reset process.
 */
void a0_reset(unsigned int frequency) {
    CCTL0 &= ~CCIE; // Timer A interrupt disabled (CCTL0 is TACCTL0)
    a0_start(frequency);
}

/**
 * Stop the timer (interrupts disabled).
 */
void a0_stop() {
    CCTL0 &= ~CCIE; // Timer A interrupt disabled (CCTL0 is TACCTL0)
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
    __bic_SR_register_on_exit(LPM0_bits); //wake up from sleep
}
