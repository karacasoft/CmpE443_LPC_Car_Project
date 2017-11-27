/*
 * ldr.h
 *
 *  Created on: Nov 26, 2017
 *      Author: karacasoft
 */

#ifndef DEV_LDR_H_
#define DEV_LDR_H_

#include <stdint.h>

extern void ldr_init();

extern void ldr_setup_left(uint8_t adc_channel);
extern void ldr_setup_right(uint8_t adc_channel);

extern uint16_t ldr_measure_left();
extern uint16_t ldr_measure_right();


#endif /* DEV_LDR_H_ */
