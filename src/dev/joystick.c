/*
 * joystick.c
 *
 *  Created on: Nov 27, 2017
 *      Author: karacasoft
 */

#include "joystick.h"
#include "iocon.h"
#include "../gpio/gpio.h"

void joystick_init() {
	IOCON_P5[0].fields.FUNC = 0;
	IOCON_P5[1].fields.FUNC = 0;
	IOCON_P5[2].fields.FUNC = 0;
	IOCON_P5[3].fields.FUNC = 0;
	IOCON_P5[4].fields.FUNC = 0;

	pinMode(5, 0, INPUT);
	pinMode(5, 1, INPUT);
	pinMode(5, 2, INPUT);
	pinMode(5, 3, INPUT);
	pinMode(5, 4, INPUT);
}

uint8_t joystick_is_button_pressed(int joy_button) {
	return readPin(5, joy_button);
}
