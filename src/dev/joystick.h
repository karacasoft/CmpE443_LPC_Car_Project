/*
 * joystick.h
 *
 *  Created on: Nov 27, 2017
 *      Author: karacasoft
 */

#ifndef DEV_JOYSTICK_H_
#define DEV_JOYSTICK_H_

#include <stdint.h>

#define JOY_BUTTON_LEFT 0
#define JOY_BUTTON_DOWN 1
#define JOY_BUTTON_UP 2
#define JOY_BUTTON_CENTER 3
#define JOY_BUTTON_RIGHT 4

extern void joystick_init();

extern uint8_t joystick_is_button_pressed(int joy_button);

#endif /* DEV_JOYSTICK_H_ */
