/*
 * adc.c
 *
 *  Created on: Nov 24, 2017
 *      Author: karacasoft
 */
#include <stdint.h>
#include <stdarg.h>

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include "adc.h"

static command_t commands[3];

static adc_callback_t callbacks[8];
static uint8_t adc_callback_state[8] = {0};

static uint8_t adc_working = 0;

adc_regs_dr_t* adc_dr_regs[8] = {
    (adc_regs_dr_t *) &ADC->DR0,
	(adc_regs_dr_t *) &ADC->DR1,
	(adc_regs_dr_t *) &ADC->DR2,
	(adc_regs_dr_t *) &ADC->DR3,
	(adc_regs_dr_t *) &ADC->DR4,
	(adc_regs_dr_t *) &ADC->DR5,
	(adc_regs_dr_t *) &ADC->DR6,
	(adc_regs_dr_t *) &ADC->DR7,
};

static struct {
	uint32_t state;

	device_info_t info;
} __adc;

struct {
	iocon_reg_t *iocon_reg;
	uint8_t iocon_func_val;
	uint8_t port;
	uint8_t pin;

} adc_iocon_values[8];

static inline void adc_start() {
	ADC_CR->PDN = 1;
	ADC_CR->START = 1;
	adc_working = 1;

	NVIC_EnableIRQ(ADC_IRQn);
}

static inline void adc_stop() {
	ADC_CR->START = 0;
	ADC_CR->PDN = 0;
	adc_working = 0;

	NVIC_DisableIRQ(ADC_IRQn);
}

static inline void adc_enable_channel(uint8_t channel) {
	SET_PIN_USED(adc_iocon_values[channel].port, adc_iocon_values[channel].pin);
	adc_iocon_values[channel].iocon_reg->fields.FUNC = adc_iocon_values[channel].iocon_func_val;
	adc_iocon_values[channel].iocon_reg->fields.MODE = 0;
	adc_iocon_values[channel].iocon_reg->fields.ADMODE = 0;

	ADC->INTEN |= (1 << channel);
}

static inline void adc_disable_channel(uint8_t channel) {
	SET_PIN_CLEARED(adc_iocon_values[channel].port, adc_iocon_values[channel].pin);
	adc_iocon_values[channel].iocon_reg->fields.FUNC = 0;
	adc_iocon_values[channel].iocon_reg->fields.MODE = 2;
	adc_iocon_values[channel].iocon_reg->fields.ADMODE = 1;

	ADC->INTEN &= ~(1 << channel);
}

volatile uint8_t current_channel = 0x00;

void ADC_IRQHandler(void) {
	if(adc_callback_state[current_channel] == ADC_CONVERT_ONCE)
	{
		adc_callback_state[current_channel] = ADC_CONVERT_CHANNEL_FREE;
		adc_disable_channel(current_channel);
	}

	uint16_t measurement = adc_dr_regs[current_channel]->RESULT;

	callbacks[current_channel](measurement);

	uint8_t beginning_channel = current_channel;

	while(!adc_callback_state[++current_channel]) {
		if(beginning_channel == current_channel) {
			adc_stop();
			return;
		}
		if(current_channel >= 7) current_channel = 0xff;
	}

	ADC_CR->SEL = (1 << current_channel);
}

/**
 * arg1 = channel (0-7)
 * (adc_callback_t) arg2 = callback to use when the value is calculated
 */
static void adc_command_convert(uint8_t val, ...) {
	va_list args;

	va_start(args, val);

	adc_callback_t callback = (adc_callback_t) va_arg(args, adc_callback_t);

	callbacks[val] = callback;

	adc_enable_channel(val);

	adc_callback_state[val] = ADC_CONVERT_ONCE;

	if(!adc_working) {
		current_channel = val;
		ADC_CR->SEL = (1 << val);
		adc_start();
	}

	va_end(args);
}

/**
 * arg1 = channel (0-7)
 * (adc_callback_t) arg2 = callback to use when the value is calculated
 */
static void adc_command_convert_continous(uint8_t val, ...) {
	va_list args;

	va_start(args, val);

	adc_callback_t callback = (adc_callback_t) va_arg(args, adc_callback_t);

	callbacks[val] = callback;

	adc_enable_channel(val);

	adc_callback_state[val] = ADC_CONVERT_CONTINOUS;

	if(!adc_working) {
		current_channel = val;
		ADC_CR->SEL = (1 << val);
		adc_start();
	}

	va_end(args);
}

/**
 * arg1 = channel (0-7)
 */
static void adc_command_stop_convert(uint8_t val, ...) {
	adc_disable_channel(val);
}

static void init(void) {
	__adc.state = 0;
	__adc.info.state = __adc.state;

	adc_iocon_values[0].iocon_reg = &IOCON_P0[23];
	adc_iocon_values[0].iocon_func_val = 1;
	adc_iocon_values[0].port = 0;
	adc_iocon_values[0].pin = 23;

	adc_iocon_values[1].iocon_reg = &IOCON_P0[24];
	adc_iocon_values[1].iocon_func_val = 1;
	adc_iocon_values[1].port = 0;
	adc_iocon_values[1].pin = 24;

	adc_iocon_values[2].iocon_reg = &IOCON_P0[25];
	adc_iocon_values[2].iocon_func_val = 1;
	adc_iocon_values[2].port = 0;
	adc_iocon_values[2].pin = 25;

	adc_iocon_values[3].iocon_reg = &IOCON_P0[26];
	adc_iocon_values[3].iocon_func_val = 1;
	adc_iocon_values[3].port = 0;
	adc_iocon_values[3].pin = 26;

	adc_iocon_values[4].iocon_reg = &IOCON_P1[30];
	adc_iocon_values[4].iocon_func_val = 3;
	adc_iocon_values[4].port = 1;
	adc_iocon_values[4].pin = 30;

	adc_iocon_values[5].iocon_reg = &IOCON_P1[31];
	adc_iocon_values[5].iocon_func_val = 3;
	adc_iocon_values[5].port = 1;
	adc_iocon_values[5].pin = 31;

	adc_iocon_values[6].iocon_reg = &IOCON_P0[12];
	adc_iocon_values[6].iocon_func_val = 3;
	adc_iocon_values[6].port = 0;
	adc_iocon_values[6].pin = 12;

	adc_iocon_values[7].iocon_reg = &IOCON_P0[13];
	adc_iocon_values[7].iocon_func_val = 3;
	adc_iocon_values[7].port = 0;
	adc_iocon_values[7].pin = 13;

	__adc.state |= DEVICE_STATE_INITIALIZED;
	__adc.info.state = __adc.state;
}

static void start(void) {
	if(!IS_DEVICE_INITIALIZED(&__adc.info)) {
		return;
	}
	USING_PERIPHERAL(PR_ADC);

	ADC->CR = ((1 << 0) | (119 << 8));


}

static void end(void) {

	ADC->CR &= ~(1 << 21);
	END_USING_PERIPHERAL(PR_ADC);
}



device_info_t *getADCDevice(void) {
	commands[ADC_COMMAND_CONVERT].execute = adc_command_convert;
	commands[ADC_COMMAND_CONVERT_CONTINOUS].execute = adc_command_convert_continous;
	commands[ADC_COMMAND_STOP_CONVERT].execute = adc_command_stop_convert;

	init();
	__adc.info.start = start;
	__adc.info.end = end;

	__adc.info.commands = commands;

	return &__adc.info;
}
