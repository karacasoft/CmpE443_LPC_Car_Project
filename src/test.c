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

#include "Ultrasonic.h"

device_info_t *rgb_led;

static uint8_t led_light;

void adc_callback_green(uint16_t value) {
	uint32_t greenVal = (uint32_t) value * 255 / 0xFFF;
	rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute((uint8_t) greenVal - 150);
}

void adc_callback_blue(uint16_t value) {
	uint32_t blueVal = (uint32_t) value * 255 / 0xFFF;
	rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute((uint8_t) blueVal - 150);
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

    //*((volatile uint32_t *) 0x400FC1A8) = 0x8;

//    device_info_t *motor_driver = getL298NDevice();
//
//    motor_driver->start();
//
//    motor_driver->commands[L298N_COMMAND_SET_SPEED].execute(40);
//    motor_driver->commands[L298N_COMMAND_GO_FORWARD].execute(0);

    init_timer();


    rgb_led = getRGBLed();

    rgb_led->start();
    rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(0);
    rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(0);
    rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(0);


//    device_info_t *adc_device = getADCDevice();
//
//    adc_device->start();
//
//    while(1) {
//    	adc_device->commands[ADC_COMMAND_CONVERT].execute(1, adc_callback_green);
//    	__WFI();
//    	adc_device->commands[ADC_COMMAND_CONVERT].execute(2, adc_callback_blue);
//    	__WFI();
//    }

//    car_init();
//    car_run();

    Ultrasonic_Capture_Timer_Init();
    Ultrasonic_Trigger_Timer_Init();

    Ulrasonic_Start_Trigger();

    while(1) {
    	if(ultrasonicSensorEdgeCount == 2) {
    		uint32_t distance = ultrasonicSensorDistance;
    		if(distance < 5) {
    			rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(125);
    			rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(0);
    			rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(0);
    		} else if(distance < 10) {
    			rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(0);
    			rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(125);
    			rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(0);
    		} else if(distance < 15) {
    			rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(0);
    			rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(0);
    			rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(125);
    		} else {
    			rgb_led->commands[RGB_LED_COMMAND_SET_RED_VALUE].execute(125);
    			rgb_led->commands[RGB_LED_COMMAND_SET_GREEN_VALUE].execute(125);
    			rgb_led->commands[RGB_LED_COMMAND_SET_BLUE_VALUE].execute(125);
    		}
    		ultrasonicSensorEdgeCount = 0;
    	}
    }

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter

    while(1) {
        i++ ;
    }
    return 0 ;
}
