/*
 * l298n.c
 *
 *  Created on: Nov 24, 2017
 *      Author: karacasoft
 */
#include <stdint.h>
#include <stdarg.h>

#include "pwm0.h"
#include "l298n.h"
#include "../gpio/gpio.h"

static command_t commands[5];

static l298n_config_t default_config;

static struct {
	uint32_t state;

	l298n_config_t config;

	device_info_t *pwm0;

	device_info_t info;
} __l298n;

static l298n_config_t get_default_config(void) {
	static uint8_t default_config_set = 0;
	if(!default_config_set) {
		default_config.pwm_channel_en_a = 5;
		default_config.pwm_channel_en_b = 6;

		default_config.port_in1 = 0;
		default_config.pin_in1 = 0;

		default_config.port_in2 = 0;
		default_config.pin_in2 = 1;

		default_config.port_in3 = 0;
		default_config.pin_in3 = 9;

		default_config.port_in4 = 0;
		default_config.pin_in4 = 8;
	}
	return default_config;
}

/**
 * uint8_t value = Speed value : (0 - 100) inclusive
 * arg2 = wheels (L298N_LEFT_WHEELS, L298N_RIGHT_WHEELS, L298N_BOTH_WHEELS)
 */
static void l298n_command_set_speed(uint8_t value, ...) {
	va_list args;

	va_start(args, value);

	int arg3 = va_arg(args, int);

	if(arg3 & L298N_LEFT_WHEELS) {
		__l298n.pwm0->commands[PWM0_COMMAND_SET_DUTY_CYCLE].execute(value, __l298n.config.pwm_channel_en_a);
	}
	if(arg3 & L298N_RIGHT_WHEELS) {
		__l298n.pwm0->commands[PWM0_COMMAND_SET_DUTY_CYCLE].execute(value, __l298n.config.pwm_channel_en_b);
	}

	va_end(args);
}

/**
 * value = 0
 */
static void l298n_command_go_forward(uint8_t value, ...) {
	writePin(__l298n.config.port_in1, __l298n.config.pin_in1, LOW);
	writePin(__l298n.config.port_in2, __l298n.config.pin_in2, HIGH);
	writePin(__l298n.config.port_in3, __l298n.config.pin_in3, LOW);
	writePin(__l298n.config.port_in4, __l298n.config.pin_in4, HIGH);
}

/**
 * value = 0
 */
static void l298n_command_go_backward(uint8_t value, ...) {
	writePin(__l298n.config.port_in1, __l298n.config.pin_in1, HIGH);
	writePin(__l298n.config.port_in2, __l298n.config.pin_in2, LOW);
	writePin(__l298n.config.port_in3, __l298n.config.pin_in3, HIGH);
	writePin(__l298n.config.port_in4, __l298n.config.pin_in4, LOW);
}

/**
 * value = 0
 */
static void l298n_command_turn_left(uint8_t value, ...) {
	writePin(__l298n.config.port_in1, __l298n.config.pin_in1, HIGH);
	writePin(__l298n.config.port_in2, __l298n.config.pin_in2, LOW);
	writePin(__l298n.config.port_in3, __l298n.config.pin_in3, LOW);
	writePin(__l298n.config.port_in4, __l298n.config.pin_in4, HIGH);
}

/**
 * value = 0
 */
static void l298n_command_turn_right(uint8_t value, ...) {
	writePin(__l298n.config.port_in1, __l298n.config.pin_in1, LOW);
	writePin(__l298n.config.port_in2, __l298n.config.pin_in2, HIGH);
	writePin(__l298n.config.port_in3, __l298n.config.pin_in3, HIGH);
	writePin(__l298n.config.port_in4, __l298n.config.pin_in4, LOW);
}

/**
 * value = 0
 */
static void l298n_command_stop(uint8_t value, ...) {
	writePin(__l298n.config.port_in1, __l298n.config.pin_in1, LOW);
	writePin(__l298n.config.port_in2, __l298n.config.pin_in2, LOW);
	writePin(__l298n.config.port_in3, __l298n.config.pin_in3, LOW);
	writePin(__l298n.config.port_in4, __l298n.config.pin_in4, LOW);
}

static void init(void) {
	if(IS_DEVICE_INITIALIZED(&__l298n.info)) return;

	__l298n.state = 0;
	__l298n.info.state = __l298n.state;
	__l298n.config = get_default_config();

	__l298n.pwm0 = getPWM0Device();

	__l298n.state |= DEVICE_STATE_INITIALIZED;
	__l298n.info.state = __l298n.state;
}

static void start(void) {
	if(!IS_DEVICE_INITIALIZED(&__l298n.info)) {
		return;
	}
	USING_PERIPHERAL(PR_PWM0);
	USING_PERIPHERAL(PR_GPIO);

	SET_PIN_USED(__l298n.config.port_in1, __l298n.config.pin_in1);
	SET_PIN_USED(__l298n.config.port_in2, __l298n.config.pin_in2);
	SET_PIN_USED(__l298n.config.port_in3, __l298n.config.pin_in3);
	SET_PIN_USED(__l298n.config.port_in4, __l298n.config.pin_in4);

	pinMode(__l298n.config.port_in1, __l298n.config.pin_in1, OUTPUT);
	pinMode(__l298n.config.port_in2, __l298n.config.pin_in2, OUTPUT);
	pinMode(__l298n.config.port_in3, __l298n.config.pin_in3, OUTPUT);
	pinMode(__l298n.config.port_in4, __l298n.config.pin_in4, OUTPUT);

	writePin(__l298n.config.port_in1, __l298n.config.pin_in1, LOW);
	writePin(__l298n.config.port_in2, __l298n.config.pin_in2, LOW);
	writePin(__l298n.config.port_in3, __l298n.config.pin_in3, LOW);
	writePin(__l298n.config.port_in4, __l298n.config.pin_in4, LOW);

	__l298n.pwm0->start();

	__l298n.pwm0->commands[PWM0_COMMAND_SET_CHANNEL_STATUS].execute(__l298n.config.pwm_channel_en_a, 1);
	__l298n.pwm0->commands[PWM0_COMMAND_SET_CHANNEL_STATUS].execute(__l298n.config.pwm_channel_en_b, 1);

	__l298n.state |= DEVICE_STATE_AVAILABLE;
	__l298n.info.state = __l298n.state;
}

static void end(void) {
	if(!IS_DEVICE_AVAILABLE(&__l298n.info)) {
		return;
	}

	SET_PIN_CLEARED(__l298n.config.port_in1, __l298n.config.pin_in1);
	SET_PIN_CLEARED(__l298n.config.port_in2, __l298n.config.pin_in2);
	SET_PIN_CLEARED(__l298n.config.port_in3, __l298n.config.pin_in3);
	SET_PIN_CLEARED(__l298n.config.port_in4, __l298n.config.pin_in4);


	__l298n.pwm0->end();

	END_USING_PERIPHERAL(PR_GPIO);
	END_USING_PERIPHERAL(PR_PWM0);
}


device_info_t *getL298NDevice(void) {
	commands[L298N_COMMAND_SET_SPEED].execute = l298n_command_set_speed;
	commands[L298N_COMMAND_GO_FORWARD].execute = l298n_command_go_forward;
	commands[L298N_COMMAND_GO_BACKWARD].execute = l298n_command_go_backward;
	commands[L298N_COMMAND_TURN_LEFT].execute = l298n_command_turn_left;
	commands[L298N_COMMAND_TURN_RIGHT].execute = l298n_command_turn_right;
	commands[L298N_COMMAND_STOP].execute = l298n_command_stop;

	init();
	__l298n.info.start = start;
	__l298n.info.end = end;

	__l298n.info.commands = commands;

	return &__l298n.info;

}
