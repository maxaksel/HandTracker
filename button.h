/**
 * Button input library.
 *
 * @date 04/11/2022
 */
#include <stdbool.h>

#ifndef BUTTON_H_
#define BUTTON_H_

/**
 * Function prototypes.
 */
void button_setup();
void enable_button_input();
void disable_button_input();
bool get_button_event_flag();
void clear_button_event_flag();
int get_button_pressed();

#endif
