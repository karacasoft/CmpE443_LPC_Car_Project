/*
 * car_leds.h
 *
 *  Created on: Nov 26, 2017
 *      Author: karacasoft
 */

#ifndef DEV_CAR_LEDS_H_
#define DEV_CAR_LEDS_H_

#define LED_FRONT_LEFT 0
#define LED_FRONT_RIGHT 1
#define LED_BACK_LEFT 2
#define LED_BACK_RIGHT 3

extern void car_leds_initialize();

extern void car_leds_set_on(uint8_t led);
extern void car_leds_set_off(uint8_t led);

extern void car_leds_blink(uint8_t led, uint32_t timeout);

#endif /* DEV_CAR_LEDS_H_ */
