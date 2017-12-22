/*
===============================================================================
 Name        : test.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

#include "gpio/gpio.h"

#include "dev/rgb_led.h"
#include "dev/l298n.h"
#include "dev/adc.h"
#include "dev/car_leds.h"
#include "car.h"
#include "dev/trimpot.h"
#include "dev/joystick.h"
#include "dev/uart.h"
#include "dev/timer.h"
#include "dev/esp8266.h"

#include "Ultrasonic.h"

#include "commandparser.h"
#include "dev/board_leds.h"

device_info_t *rgb_led;

uint16_t value;

uint8_t reading_ldr = 0;
uint32_t greenVal;

#define TEST_MOTOR 0
#define TEST_LED 1
#define TEST_ULTRASONIC 2
#define TEST_LDR 3
#define TEST_TRIMPOT 4
#define TEST_CAR 5
#define DEMO_DAY 6
#define TEST_LDR_INTENSITY 7
#define TEST_COMMANDS 8
#define TEST_JOYSTICK 9

uint8_t run_config = TEST_CAR;

void adc_callback_green(uint16_t value) {
	greenVal = (uint32_t) value * 255 / 0xFFF;
	rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute((uint8_t) greenVal - 150);
	reading_ldr = 0;
}

void adc_callback_blue(uint16_t value) {
	uint32_t blueVal = (uint32_t) value * 255 / 0xFFF;
	rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute((uint8_t) blueVal - 150);
}

void test_ultrasonic() {
	rgb_led = getRGBLed();

	rgb_led->start();
	rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(0);
	rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(0);
	rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(0);

	Ultrasonic_Capture_Timer_Init();
	Ultrasonic_Trigger_Timer_Init();

	Ultrasonic_Start_Trigger();

	while(1) {
		if(ultrasonicSensorEdgeCount == 2) {
			uint32_t distance = ultrasonicSensorDistance;
			if(distance < 10) {
				rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(125);
				rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(0);
				rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(0);
			} else if(distance < 20) {
				rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(0);
				rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(125);
				rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(0);
			} else if(distance < 30) {
				rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(0);
				rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(0);
				rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(125);
			} else {
				rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(125);
				rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(125);
				rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(125);
			}
		}
	}
}

void test_trimpot() {
	rgb_led = getRGBLed();

	rgb_led->start();
	rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(0);
	rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(0);
	rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(0);


	trimpot_init();

	while(1) {
		value = trimpot_measure();

		rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute((value * 255 / 0xFFF));
	}
}

void test_leds() {
	car_leds_initialize();

	car_leds_set_on(LED_FRONT_LEFT);
	car_leds_set_on(LED_FRONT_RIGHT);
	car_leds_set_off(LED_BACK_LEFT);
	car_leds_set_off(LED_BACK_RIGHT);

	sleep(3000);

	car_leds_set_off(LED_FRONT_LEFT);
	car_leds_set_off(LED_FRONT_RIGHT);
	car_leds_set_on(LED_BACK_LEFT);
	car_leds_set_on(LED_BACK_RIGHT);

	sleep(3000);

	car_leds_set_off(LED_FRONT_RIGHT);
	car_leds_set_off(LED_BACK_RIGHT);
	car_leds_blink(LED_FRONT_LEFT, 500);
	car_leds_blink(LED_BACK_LEFT, 500);

	sleep(3000);

	car_leds_set_off(LED_FRONT_LEFT);
	car_leds_set_off(LED_BACK_LEFT);
	car_leds_blink(LED_FRONT_RIGHT, 500);
	car_leds_blink(LED_BACK_RIGHT, 500);
}

void test_ldr() {
	rgb_led = getRGBLed();

	rgb_led->start();
	rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(0);
	rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(0);
	rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(0);


	device_info_t *adc_device = getADCDevice();

	adc_device->start();

	while(1) {
		reading_ldr = 1;
		adc_device->commands[ADC_COMMAND_CONVERT].execute(2, adc_callback_green);
		while(reading_ldr) {
			__WFI();
		}
		//    	adc_device->commands[ADC_COMMAND_CONVERT].execute(3, adc_callback_blue);
		//    	__WFI();
	}
}

uint32_t difference;
uint8_t direction;
uint16_t lightLeft;
uint16_t lightRight;

void test_ldr_intensity() {
	ldr_init();
	while(1) {
		lightLeft = ldr_measure_left();
		lightRight = ldr_measure_right();

		if(lightLeft > lightRight) {
			difference = lightLeft - lightRight;
			direction = 0;
		} else {
			difference = lightRight - lightLeft;
			direction = 1;
		}
	}
}

void start_car(){
	//VROOOOM
	car_init();
	car_run();
}

void test_motor() {
	device_info_t *motor_driver = getL298NDevice();

	motor_driver->start();

	motor_driver->commands[L298N_COMMAND_SET_SPEED].execute(90, L298N_BOTH_WHEELS);
	motor_driver->commands[L298N_COMMAND_GO_FORWARD].execute(0);
	sleep(3000);
	motor_driver->commands[L298N_COMMAND_GO_BACKWARD].execute(0);

}

void test_command_parse() {
	char command[] = "ID02N15W20";
	movement_command_t* temp = parse_command(command);
	if(temp->direction1 == DIR_NORTH) {

	} else if(temp->direction1 == DIR_SOUTH) {

	} else if(temp->direction1 == DIR_EAST) {

	} else if(temp->direction1 == DIR_WEST) {

	}
}

void test_joystick() {
	rgb_led = getRGBLed();

	rgb_led->start();
	rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(0);
	rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(0);
	rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(0);

	joystick_init();

	while(1) {
		if(joystick_is_button_pressed(JOY_BUTTON_CENTER)) {
			rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(125);
			rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(0);
			rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(0);
		} else if(joystick_is_button_pressed(JOY_BUTTON_UP)) {
			rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(0);
			rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(125);
			rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(0);
		} else if(joystick_is_button_pressed(JOY_BUTTON_DOWN)) {
			rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(0);
			rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(0);
			rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(125);
		} else if(joystick_is_button_pressed(JOY_BUTTON_LEFT)) {
			rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(125);
			rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(125);
			rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(125);
		} else if(joystick_is_button_pressed(JOY_BUTTON_RIGHT)) {
			rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(255);
			rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(255);
			rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(255);
		} else {
			rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(0);
			rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(0);
			rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(0);
		}
	}
}

void (*func_list[])(void) = {
		test_motor,
		test_leds,
		test_ultrasonic,
		test_ldr,
		test_trimpot,
		start_car,
		start_car,
		test_ldr_intensity,
		test_command_parse,
		test_joystick
};

void uart_buffer_callback_for_test(char *recString) {
	uart_context_t *context = uart_getUartContext(0);
	
	uart_send(context, *recString);
}

void test_uart() {
	uart_context_t *context = uart_getUartContext(0);
	uart_setBaudRate(context, 9600);
	uart_init(context);
	
	uart_setBufferCompleteCallback(context, uart_buffer_callback_for_test);
	
	uart_sendString(context, "Hello world!", 12);
	
}

int str_length(char *str) {
	int i = 0;
	while(str[i++] != 0);
	return i;
}

void on_esp_response(char *response) {
	uart_context_t *context = uart_getUartContext(0);
	
	uart_sendString(context, response, str_length(response));
}

void uart_esp_command(char *recString) {
	static uint8_t i = 0;
	static char buffer[256];

	uart_context_t *context = uart_getUartContext(0);
		
	buffer[i++] = *recString;
	if(*recString == '\r') {
		buffer[i] = '\n';
		buffer[i + 1] = 0;
		esp8266_send(esp8266_get_context(), buffer, i + 1);
		i = 0;
		uart_send(context, '\n');
	}
	
	uart_send(context, *recString);
}

void test_esp() {
	uart_context_t *uart_context = uart_getUartContext(0);
	uart_setBaudRate(uart_context, 9600);
	uart_init(uart_context);
	
	uart_setBufferCompleteCallback(uart_context, uart_esp_command);
	
	esp_context_t *context = esp8266_get_context();
	
	esp8266_init(context);
	
	esp8266_reset(context);
	
	esp8266_setOnResponseListener(context, on_esp_response);
}

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
	// Set the LED to the state of "On"
	Board_LED_Set(0, true);
#endif
#endif

	init_timer();

	//    rgb_led = getRGBLed();
	//
	//    rgb_led->start();
	//    rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(0);
	//    rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(0);
	//    rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(0);

	// func_list[run_config]();

    test_esp();
    

	// Force the counter to be placed into memory
	volatile static int i = 0 ;
	// Enter an infinite loop, just incrementing a counter

	while(1) {
		i++ ;
	}
	return 0 ;
}
