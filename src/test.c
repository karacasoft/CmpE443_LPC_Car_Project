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

#include "Ultrasonic.h"

#include "commandparser.h"

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

uint8_t run_config = DEMO_DAY;

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

void (*func_list[7])(void) = {
	test_motor,
	test_leds,
	test_ultrasonic,
	test_ldr,
	test_trimpot,
	start_car,
	start_car
};

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

//    func_list[run_config]();

    char command[] = "ID02N15W20";
    movement_command_t* temp = parse_command(command);

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter

    while(1) {
        i++ ;
    }
    return 0 ;
}
