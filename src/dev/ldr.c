/*
 * ldr.c
 *
 *  Created on: Nov 26, 2017
 *      Author: karacasoft
 */
#include "chip.h"

#include <stdint.h>
#include "device.h"
#include "adc.h"
#include "ldr.h"

device_info_t *ldr_adc_device;

uint8_t ldr_channel_left = 2;
uint8_t ldr_channel_right = 3;

uint8_t adc_is_measuring = 0;

uint16_t ldr_value;

void adc_callback(uint16_t value) {
	ldr_value = value;
	adc_is_measuring = 0;
}

void ldr_init() {
	ldr_adc_device = getADCDevice();
	ldr_adc_device->start();
}

void ldr_setup_left(uint8_t adc_channel) {
	ldr_channel_left = adc_channel;
}

void ldr_setup_right(uint8_t adc_channel) {
	ldr_channel_right = adc_channel;
}

uint16_t ldr_measure_left() {
	adc_is_measuring = 1;

	ldr_adc_device->commands[ADC_COMMAND_CONVERT].execute(ldr_channel_left, adc_callback);
	while(adc_is_measuring) {
		__WFI();
	}
	return ldr_value;
}

uint16_t ldr_measure_right() {
	adc_is_measuring = 1;

	ldr_adc_device->commands[ADC_COMMAND_CONVERT].execute(ldr_channel_right, adc_callback);
	while(adc_is_measuring) {
		__WFI();
	}
	return ldr_value;
}
