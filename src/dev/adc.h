/*
 * adc.h
 *
 *  Created on: Nov 24, 2017
 *      Author: karacasoft
 */

#ifndef DEV_ADC_H_
#define DEV_ADC_H_

#include <stdint.h>
#include "iocon.h"
#include "device.h"


typedef struct {
	volatile uint32_t SEL : 8;
	volatile uint32_t CLKDIV : 8;
	volatile uint32_t BURST : 1;
			 uint32_t RESERVED0 : 4;
	volatile uint32_t PDN : 1;
			 uint32_t RESERVED1 : 2;
	volatile uint32_t START : 3;
	volatile uint32_t EDGE : 1;
			 uint32_t RESERVED2 : 4;
} adc_regs_cr_t;

typedef struct {
			 uint32_t RESERVED0 : 4;
	volatile uint32_t RESULT : 12;
			 uint32_t RESERVED1 : 8;
	volatile uint32_t CHN : 3;
			 uint32_t RESERVED2 : 3;
	volatile uint32_t OVERRUN : 1;
	volatile uint32_t DONE : 1;
} adc_regs_dr_t;

typedef struct {
	volatile uint32_t CR;
	volatile uint32_t GDR;
			 uint32_t RESERVED0;
	volatile uint32_t INTEN;
	volatile uint32_t DR0;
	volatile uint32_t DR1;
	volatile uint32_t DR2;
	volatile uint32_t DR3;
	volatile uint32_t DR4;
	volatile uint32_t DR5;
	volatile uint32_t DR6;
	volatile uint32_t DR7;
	volatile uint32_t STAT;
	volatile uint32_t TRM;
} adc_regs_t;

#define ADC_BASE 0x40034000
#define ADC ((adc_regs_t *) ADC_BASE)

extern adc_regs_dr_t* adc_dr_regs[8];

#define ADC_CR ((adc_regs_cr_t *) &(ADC->CR))

typedef void (*adc_callback_t)(uint16_t);

extern iocon_reg_t *adc_iocon_regs_lut[8];


#define ADC_COMMAND_CONVERT 0
#define ADC_COMMAND_CONVERT_CONTINOUS 1
#define ADC_COMMAND_STOP_CONVERT 2

#define ADC_CONVERT_CHANNEL_FREE 0
#define ADC_CONVERT_ONCE 1
#define ADC_CONVERT_CONTINOUS 2


device_info_t *getADCDevice(void);

#endif /* DEV_ADC_H_ */
