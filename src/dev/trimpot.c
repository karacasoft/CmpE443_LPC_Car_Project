/*
 * trimpot.c
 *
 *  Created on: Nov 27, 2017
 *      Author: karacasoft
 */

#include "chip.h"

#include "device.h"
#include "trimpot.h"
#include "adc.h"

volatile uint8_t measuring_trimpot = 0;

device_info_t *trimpot_adc_device;

uint16_t last_value;

void measure_callback(uint16_t value) {
	last_value = value;
	measuring_trimpot = 0;
}

void trimpot_init() {
	trimpot_adc_device = getADCDevice();
	trimpot_adc_device->start();
}

uint16_t trimpot_measure() {
	measuring_trimpot = 1;
	trimpot_adc_device->commands[ADC_COMMAND_CONVERT].execute(0, measure_callback);
	while(measuring_trimpot) {
		__WFI();
	}
	return last_value;
}

