#ifndef __L298N_H__
#define __L298N_H__

#include "device.h"

#define L298N_LEFT_WHEELS 1
#define L298N_RIGHT_WHEELS 2
#define L298N_BOTH_WHEELS (L298N_LEFT_WHEELS | L298N_RIGHT_WHEELS)

#define L298N_COMMAND_SET_SPEED 0
#define L298N_COMMAND_GO_FORWARD 1
#define L298N_COMMAND_GO_BACKWARD 2
#define L298N_COMMAND_TURN_LEFT 3
#define L298N_COMMAND_TURN_RIGHT 4
#define L298N_COMMAND_STOP 5

typedef struct {
	uint8_t pwm_channel_en_a;
	uint8_t pwm_channel_en_b;

	uint8_t port_in1 : 3;
	uint8_t pin_in1 : 5;

	uint8_t port_in2 : 3;
	uint8_t pin_in2 : 5;

	uint8_t port_in3 : 3;
	uint8_t pin_in3 : 5;

	uint8_t port_in4 : 3;
	uint8_t pin_in4 : 5;
} l298n_config_t;

device_info_t *getL298NDevice(void);



#endif
