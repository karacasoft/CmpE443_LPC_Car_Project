#include <stdarg.h>

#include "pwm0.h"

#include "iocon.h"
#include "device.h"
#include "../peripherals/peripherals.h"

static struct {
	uint32_t state;
	uint32_t period;
	device_info_t info;
	pwm_channel_t channels[6];
} pwm0_dev;

static command_t commands[3];

/**
 * val(uint8_t) = 0-100 duty cycle in %
 * channel(uint8_t) = channel number(1-6)
 */
static void pwm0_set_duty_cycle(uint8_t val, ...) {
	if(val > 100) val = 101;

	va_list args;

	va_start(args, val);

	// char is promoted to int while passing through (...)
	uint8_t channel = (uint8_t) va_arg(args, int) - 1;

	*(pwm0_dev.channels[channel].duty_cycle_register) = val * pwm0_dev.period / 100;

	PWM0->LER |= (1 << (channel + 1));

	va_end(args);
}

/**
 * val(uint8_t) = value type
 * period(uint32_t) = value
 */
static void pwm0_set_period(uint8_t val, ...) {
	va_list args;

	va_start(args, val);

	uint32_t period = va_arg(args, uint32_t);
	if(val == VALUE_TYPE_RAW_CYCLES) {
		PWM0->MR0 = period;
		pwm0_dev.period = period;

		PWM0->LER |= (1 << 0);
	}

	va_end(args);
}

/**
 * val(uint8_t) = channel number = 1
 * status(uint8_t) = is channel used
 */
static void pwm0_set_channel_status(uint8_t val, ...) {
	va_list args;

	va_start(args, val);

	// char is promoted to int while passing through (...)
	uint8_t status = (uint8_t)va_arg(args, int);

	pwm_channel_t channel = pwm0_dev.channels[val - 1];

	if(status == 0) {
		SET_PIN_CLEARED(channel.port, channel.pin);
	} else {
		SET_PIN_USED(channel.port, channel.pin);
		IOCON_PORTS[channel.port][channel.pin].fields.FUNC = 0x03;

		PWM0->PCR |= (1 << (8 + val));
	}

	channel.available = !(status);

	va_end(args);
}

static void init(void) {
	// nullify device state
	pwm0_dev.state = 0;
	pwm0_dev.info.state = pwm0_dev.state;

	// setup pwm channels
	pwm0_dev.channels[0].port = 1;
	pwm0_dev.channels[0].pin = 2;
	pwm0_dev.channels[0].registers = PWM0;
	pwm0_dev.channels[0].duty_cycle = 0;
	pwm0_dev.channels[0].duty_cycle_register = &(PWM0->MR1);
	pwm0_dev.channels[0].available = 1;

	pwm0_dev.channels[1].port = 1;
	pwm0_dev.channels[1].pin = 3;
	pwm0_dev.channels[1].registers = PWM0;
	pwm0_dev.channels[1].duty_cycle = 0;
	pwm0_dev.channels[1].duty_cycle_register = &(PWM0->MR2);
	pwm0_dev.channels[1].available = 1;

	pwm0_dev.channels[2].port = 1;
	pwm0_dev.channels[2].pin = 5;
	pwm0_dev.channels[2].registers = PWM0;
	pwm0_dev.channels[2].duty_cycle = 0;
	pwm0_dev.channels[2].duty_cycle_register = &(PWM0->MR3);
	pwm0_dev.channels[2].available = 1;

	pwm0_dev.channels[3].port = 1;
	pwm0_dev.channels[3].pin = 6;
	pwm0_dev.channels[3].registers = PWM0;
	pwm0_dev.channels[3].duty_cycle = 0;
	pwm0_dev.channels[3].duty_cycle_register = &(PWM0->MR4);
	pwm0_dev.channels[3].available = 1;

	pwm0_dev.channels[4].port = 1;
	pwm0_dev.channels[4].pin = 7;
	pwm0_dev.channels[4].registers = PWM0;
	pwm0_dev.channels[4].duty_cycle = 0;
	pwm0_dev.channels[4].duty_cycle_register = &(PWM0->MR5);
	pwm0_dev.channels[4].available = 1;

	pwm0_dev.channels[5].port = 1;
	pwm0_dev.channels[5].pin = 11;
	pwm0_dev.channels[5].registers = PWM0;
	pwm0_dev.channels[5].duty_cycle = 0;
	pwm0_dev.channels[5].duty_cycle_register = &(PWM0->MR6);
	pwm0_dev.channels[5].available = 1;

	pwm0_dev.state |= DEVICE_STATE_INITIALIZED;
	pwm0_dev.info.state = pwm0_dev.state;

}

static void start(void) {
	if(!IS_DEVICE_INITIALIZED(&pwm0_dev.info)) {
		return;
	}
	USING_PERIPHERAL(PR_PWM0);

	PWM0->TCR |= (1 << 1);

	// set period to 20ms
	pwm0_set_period(VALUE_TYPE_RAW_CYCLES, PERIPHERAL_CLOCK_FREQUENCY * 20 / 1000);

	PWM0->MCR |= (1 << 1);

	PWM0->TCR |= (1 << 0) | (1 << 3);
	// Some nops to let
	// the register to setup properly
	asm volatile("nop\nnop\nnop\n");
	PWM0->TCR &= ~(1 << 1);

	pwm0_dev.state |= DEVICE_STATE_AVAILABLE;
	pwm0_dev.info.state = pwm0_dev.state;

}

static void end(void) {
	if(!IS_DEVICE_AVAILABLE(&pwm0_dev.info)) {
		return;
	}
	END_USING_PERIPHERAL(PR_PWM0);
}

device_info_t * getPWM0Device(void) {

	commands[0].execute = pwm0_set_duty_cycle;

	commands[1].execute = pwm0_set_period;

	commands[2].execute = pwm0_set_channel_status;

	init();
	pwm0_dev.info.init = (void *) init;
	pwm0_dev.info.start = (void *) start;
	pwm0_dev.info.end = (void *) end;

	pwm0_dev.info.commands = commands;

	return &pwm0_dev.info;
}
