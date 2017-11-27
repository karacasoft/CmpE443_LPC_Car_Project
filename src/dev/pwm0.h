#ifndef __PWM0_H__
#define __PWM0_H__

#include <stdint.h>
#include "device.h"

// TODO change this value to a dynamically retrieved one
#define PERIPHERAL_CLOCK_FREQUENCY 60000000

#define PWM0_BASE 0x40014000
#define PWM1_BASE 0x40018000

#define PWM0 ((pwm_regs_t *) PWM0_BASE)
#define PWM1 ((pwm_regs_t *) PWM1_BASE)

#define VALUE_TYPE_RAW_CYCLES 1

#define PWM0_COMMAND_SET_DUTY_CYCLE      0
#define PWM0_COMMAND_SET_PERIOD          1
#define PWM0_COMMAND_SET_CHANNEL_STATUS  2

typedef struct {
	volatile uint32_t IR;
	volatile uint32_t TCR;
	volatile uint32_t TC;
	volatile uint32_t PR;
	volatile uint32_t PC;
	volatile uint32_t MCR;
	volatile uint32_t MR0;
	volatile uint32_t MR1;
	volatile uint32_t MR2;
	volatile uint32_t MR3;
	volatile uint32_t CCR;
	volatile uint32_t CR0;
	volatile uint32_t CR1;
			 uint32_t RESERVED0[3];
	volatile uint32_t MR4;
	volatile uint32_t MR5;
	volatile uint32_t MR6;
	volatile uint32_t PCR;
	volatile uint32_t LER;
			 uint32_t RESERVED1[7];
	volatile uint32_t CTCR;
} pwm_regs_t;

typedef struct {
	pwm_regs_t *registers;

	uint8_t port : 3;
	uint8_t pin : 5;

	uint8_t duty_cycle;
	volatile uint32_t *duty_cycle_register;

	uint8_t available;


} pwm_channel_t;

extern device_info_t * getPWM0Device(void);

#endif
