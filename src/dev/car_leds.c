/*
 * car_leds.c
 *
 *  Created on: Nov 26, 2017
 *      Author: karacasoft
 */

#include "timer.h"
#include "iocon.h"
#include "../gpio/gpio.h"
#include "car_leds.h"

uint8_t port_lut[4] = {
		1, 1, 1, 0
};
uint8_t pin_lut[4] ={
		24, 23, 20, 21
};

uint8_t led_light_status = 0x00;
uint8_t led_blink_status = 0x00;

uint8_t timer_ids[4];

void set_on(uint8_t led);
void set_off(uint8_t led);
void clear_blink(uint8_t led);

void led1_callback(uint32_t millis) {
	if(led_light_status & (1 << LED_FRONT_LEFT)) {
		set_off(LED_FRONT_LEFT);
	} else {
		set_on(LED_FRONT_LEFT);
	}
}

void led2_callback(uint32_t millis) {
	if(led_light_status & (1 << LED_FRONT_RIGHT)) {
		set_off(LED_FRONT_RIGHT);
	} else {
		set_on(LED_FRONT_RIGHT);
	}
}

void led3_callback(uint32_t millis) {
	if(led_light_status & (1 << LED_BACK_LEFT)) {
		set_off(LED_BACK_LEFT);
	} else {
		set_on(LED_BACK_LEFT);
	}
}

void led4_callback(uint32_t millis) {
	if(led_light_status & (1 << LED_BACK_RIGHT)) {
		set_off(LED_BACK_RIGHT);
	} else {
		set_on(LED_BACK_RIGHT);
	}
}

void car_leds_initialize() {

	IOCON_P1[24].fields.FUNC = 0;
	IOCON_P1[23].fields.FUNC = 0;
	IOCON_P1[20].fields.FUNC = 0;
	IOCON_P0[21].fields.FUNC = 0;

	int i = 0;
	for(i = 0; i < 4; i++) {
		pinMode(port_lut[i], pin_lut[i], OUTPUT);
		writePin(port_lut[i], pin_lut[i], LOW);
	}
}

void clear_blink(uint8_t led) {
	if(led_blink_status & (1 << led)) {
		led_blink_status &= ~(1 << led);
		clear_interval(timer_ids[led]);
	}
}

void set_on(uint8_t led) {
	writePin(port_lut[led], pin_lut[led], HIGH);
	led_light_status |= (1 << led);
}

void set_off(uint8_t led) {
	writePin(port_lut[led], pin_lut[led], LOW);
	led_light_status &= ~(1 << led);
}

void car_leds_set_on(uint8_t led) {
	clear_blink(led);
	set_on(led);
}

void car_leds_set_off(uint8_t led) {
	clear_blink(led);
	set_off(led);
}

void car_leds_blink(uint8_t led, uint32_t timeout) {
	if(!(led_blink_status & (1 << led))) {
		timer_callback_t callback;
		if(led == LED_FRONT_LEFT) {
			callback = led1_callback;
		} else if(led == LED_FRONT_RIGHT) {
			callback = led2_callback;
		} else if(led == LED_BACK_LEFT) {
			callback = led3_callback;
		} else if(led == LED_BACK_RIGHT) {
			callback = led4_callback;
		}
		led_blink_status |= (1 << led);
		timer_ids[led] = set_interval(timeout, callback);
	}

}
