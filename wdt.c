/**
 * Watchdog timer control library.
 *
 * @date 04/11/2022
 */
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "wdt.h"

/**
 * WDT Module Status Variables
 */
unsigned int wdt_divider;
unsigned int wdt_div_tracker;
bool wdt_flag;
bool is_paused;


/**
 * Initial watchdog setup. ACLK is sourced from the
 * VLO and the watchdog is started in the off state.
 */
void wdt_setup() {
    WDTCTL = WDTHOLD | WDTPW; // turn off watchdog
    is_paused = false;
    wdt_flag = false;
}

/**
 * Start the watchdog timer with WDT_ADLY_16 and
 * a specified divider.
 *
 * @param divider an unsigned integer >= 1
 */
void wdt_start(unsigned int divider) {
    WDTCTL = WDT_ADLY_16;  // enable watchdog timer
    IE1 |= WDTIE;   // enable WDT interrupt

    wdt_div_tracker = 0;
    wdt_divider = divider;
}

/**
 * Pauses the watchdog timer and preserves
 * all state variables.
 */
void wdt_pause() {
    is_paused = true;
}

/**
 * Resumes the watchdog timer after a pause.
 */
void wdt_resume() {
    is_paused = false;
}

/**
 * Resets the watchdog timer and all state variables.
 */
void wdt_reset(unsigned int divider) {
    wdt_flag = false;
    wdt_stop();
    wdt_start(divider);

}

/**
 * Stops the watchdog timer.
 */
void wdt_stop() {
    WDTCTL = WDTHOLD | WDTPW; // turn off watchdog
    wdt_flag = false;
}

/**
 * Returns true if WDT interrupt has been triggered and WDT is
 * not in the paused state.
 *
 * @returns boolean (unsigned char)
 */
bool get_wdt_flag() {
    return wdt_flag;
}

/**
 * Clears WDT flag.
 */
void clear_wdt_flag() {
    wdt_flag = false;
}

/**
 * Watchdog Timer interrupt service routine.
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
    if (is_paused) {
        return; // if watchdog is paused, do nothing in ISR
    }

    if (wdt_div_tracker == wdt_divider - 1) {
        wdt_flag = true;
        wdt_div_tracker = 0;
        // __bic_SR_register_on_exit(LPM3_bits); // exit LPM3 when returning to program (clear LPM3 bits)
    }

    wdt_div_tracker++;
}
