/**
 * Watchdog control library. Consumes WDT module on
 * the MSP430. 16ms default period with divider=1
 * assuming the watchdog is sourced from SMCLK.
 *
 * @date 04/11/2022
 */
#include <stdbool.h>

#ifndef WDT_H_
#define WDT_H_

/**
 * Function prototypes.
 */
void wdt_setup();
void wdt_start(unsigned int divider);
void wdt_pause();
void wdt_resume();
void wdt_reset(unsigned int divider);
void wdt_stop();
bool get_wdt_flag();
void clear_wdt_flag();

#endif
