/*
 * car.c
 *
 *  Created on: Nov 26, 2017
 *      Author: karacasoft
 */

#define LIGHT_DIFF_THRESHOLD 50

#define LIGHT_DIRECTION_LEFT 0
#define LIGHT_DIRECTION_RIGHT 1

#include "car.h"
#include "dev/timer.h"
#include "dev/ldr.h"
#include "dev/car_leds.h"
#include "dev/l298n.h"

uint8_t state;

device_info_t *motor_driver;

uint16_t speed = 50;

void evade_obstacle_behavior();
void go_forward_behavior();
void go_nuts_behavior();
void follow_light_behavior();


void check_ldr(uint16_t *difference, uint8_t *direction) {
	uint16_t lightLeft = ldr_measure_left();
	uint16_t lightRight = ldr_measure_right();

	if(lightLeft > lightRight) {
		*difference = lightLeft - lightRight;
		*direction = LIGHT_DIRECTION_LEFT;
	} else {
		*difference = lightRight - lightLeft;
		*direction = LIGHT_DIRECTION_RIGHT;
	}
}

void evade_obstacle_behavior() {
	// TODO implement Ultrasonic sensor
}

void go_forward_behavior() {

	// TODO check ultrasonic sensor value
	// change state if it should be changed


	uint16_t speed_locked = speed;
	motor_driver->commands[L298N_COMMAND_SET_SPEED].execute(speed_locked, L298N_LEFT_WHEELS);
	motor_driver->commands[L298N_COMMAND_SET_SPEED].execute(speed_locked, L298N_RIGHT_WHEELS);
}

void go_nuts_behavior() {
	//TODO should not be implemented
	while(1) {}
}

void follow_light_behavior() {
	uint8_t starting_state = state;

	//TODO check ultrasonic sensor value
	// change state if it should be changed

	uint16_t difference;
	uint8_t light_dir;

	check_ldr(&difference, &light_dir);

	if(difference > LIGHT_DIFF_THRESHOLD) {

		uint16_t speed_locked = speed;
		// This intermediate variable is set to stabilize speed.
		// It may change during an interrupt. So we use the value
		// stored in this.

		if(light_dir == LIGHT_DIRECTION_LEFT) {
			motor_driver->commands[L298N_COMMAND_SET_SPEED].execute(speed_locked, L298N_LEFT_WHEELS);
			motor_driver->commands[L298N_COMMAND_SET_SPEED].execute(speed_locked + (difference / 20), L298N_RIGHT_WHEELS);


			car_leds_set_off(LED_FRONT_RIGHT);
			car_leds_set_off(LED_BACK_RIGHT);
			car_leds_blink(LED_FRONT_LEFT, 500);
			car_leds_blink(LED_BACK_LEFT, 500);

		} else if (light_dir == LIGHT_DIRECTION_RIGHT){
			motor_driver->commands[L298N_COMMAND_SET_SPEED].execute(speed_locked + (difference / 20), L298N_LEFT_WHEELS);
			motor_driver->commands[L298N_COMMAND_SET_SPEED].execute(speed_locked, L298N_RIGHT_WHEELS);

			car_leds_set_off(LED_FRONT_LEFT);
			car_leds_set_off(LED_BACK_LEFT);
			car_leds_blink(LED_FRONT_RIGHT, 500);
			car_leds_blink(LED_BACK_RIGHT, 500);
		}
	} else {
		state = CAR_STATE_GO_FORWARD;
	}

	if(state != starting_state) {
		sleep(100);
	}
}

void car_init() {
	car_leds_initialize();
	ldr_init();

	motor_driver = getL298NDevice();
	motor_driver->start();
}

void car_run() {
	while(1) {
		switch(state) {
		case CAR_STATE_EVADE_OBSTACLE:
			evade_obstacle_behavior();
			break;
		case CAR_STATE_FOLLOW_LIGHT:
			follow_light_behavior();
			break;
		case CAR_STATE_GO_FORWARD:
			go_forward_behavior();
			break;
		default:
			go_nuts_behavior();
			break;
		}
	}
}
