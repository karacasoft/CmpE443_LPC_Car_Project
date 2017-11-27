#include <stdint.h>
#include <stdarg.h>

#include "device.h"
#include "rgb_led.h"
#include "pwm0.h"

#include "../peripherals/peripherals.h"

static command_t commands[4];

static struct {
	uint32_t state;

	device_info_t info;
	device_info_t *pwm0;
} rgb_led;

static void rgb_led_command_set_red(uint8_t value, ...) {
	rgb_led.pwm0->commands[PWM0_COMMAND_SET_DUTY_CYCLE].execute((100 - value * 100 / 255), 6);
}

static void rgb_led_command_set_green(uint8_t value, ...) {
	rgb_led.pwm0->commands[PWM0_COMMAND_SET_DUTY_CYCLE].execute((100 - value * 100 / 255), 3);
}

static void rgb_led_command_set_blue(uint8_t value, ...) {
	rgb_led.pwm0->commands[PWM0_COMMAND_SET_DUTY_CYCLE].execute((100 - value * 100 / 255), 5);
}

static void rgb_led_command_set_rgb(uint8_t value, ...) {
	va_list args;

	va_start(args, value);

	uint8_t green = (uint8_t) va_arg(args, int); // char promotes to int
	uint8_t blue = (uint8_t) va_arg(args, int); // char promotes to int

	rgb_led_command_set_red(value);
	rgb_led_command_set_green(green);
	rgb_led_command_set_blue(blue);

	va_end(args);
}

static void init(void) {
	// stop if the device is already initialized
	if(IS_DEVICE_INITIALIZED(&rgb_led.info)) return;

	rgb_led.state = 0;
	rgb_led.info.state = rgb_led.state;

	rgb_led.pwm0 = getPWM0Device();

	rgb_led.pwm0->init();

	rgb_led.state |= DEVICE_STATE_INITIALIZED;
	rgb_led.info.state = rgb_led.state;
}

static void start(void) {
	if(!IS_DEVICE_INITIALIZED(&rgb_led.info)) {
		return;
	}
	USING_PERIPHERAL(PR_PWM0);

	rgb_led.pwm0->start();

	rgb_led.pwm0->commands[PWM0_COMMAND_SET_CHANNEL_STATUS].execute(3, 1);
	rgb_led.pwm0->commands[PWM0_COMMAND_SET_CHANNEL_STATUS].execute(5, 1);
	rgb_led.pwm0->commands[PWM0_COMMAND_SET_CHANNEL_STATUS].execute(6, 1);

    rgb_led.pwm0->commands[PWM0_COMMAND_SET_DUTY_CYCLE].execute(100, 3);
    rgb_led.pwm0->commands[PWM0_COMMAND_SET_DUTY_CYCLE].execute(100, 5);
    rgb_led.pwm0->commands[PWM0_COMMAND_SET_DUTY_CYCLE].execute(100, 6);

    rgb_led.state |= DEVICE_STATE_AVAILABLE;
    rgb_led.info.state = rgb_led.state;
}

static void end(void) {
	if(!IS_DEVICE_AVAILABLE(&rgb_led.info)) {
		return;
	}
	rgb_led.pwm0->end();

	END_USING_PERIPHERAL(PR_PWM0);
}

device_info_t *getRGBLed(void) {
	commands[0].execute = rgb_led_command_set_red;
	commands[1].execute = rgb_led_command_set_green;
	commands[2].execute = rgb_led_command_set_blue;
	commands[3].execute = rgb_led_command_set_rgb;

	init();
	rgb_led.info.start = (void *) start;
	rgb_led.info.end = (void *) end;

	rgb_led.info.commands = commands;

	return &rgb_led.info;
}
