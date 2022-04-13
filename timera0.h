/**
 * Timing library. Consumes A0 module on
 * the MSP430. Assumes SMCLK as
 *
 * @date 04/11/2022
 */

#include <stdbool.h>

#ifndef WDT_H_
#define WDT_H_

/**
 * Function prototypes.
 */
void a0_setup();
void a0_start(unsigned int divider);
void a0_pause();
void a0_resume();
void a0_reset(unsigned int divider);
void a0_stop();
bool get_a0_flag();
void clear_a0_flag();

#endif
